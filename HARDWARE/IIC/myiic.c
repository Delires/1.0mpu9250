#include "myiic.h"

//ErrorStatus HSEStartUpStatus;  //����һ��ö�����͵Ľṹ�壬�ڴ˺���ûɶ�á�

char  test=0; 				 //IIC�õ�
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6  //����BSRR�Ĵ���������SCL�ߵ�ƽ
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6  //SCLΪ�͵�ƽ
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7  //SDA ��
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7  //SDA ��

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6  //��SCL PB6���ŵ�ƽ
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7  //��SDA PB7���ŵ�ƽ



//����PB6ΪSCLʹ��ʱ���    PB7ΪSDA�źŴ����
void I2C_GPIO_Config(void)  //����PB6ΪSCLʹ��ʱ���    PB7ΪSDA�źŴ����
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}



void I2C_delay(void)   //I2C����ʱ����
{
		
   u8 i=30; //��������Ż��ٶ�	����������͵�5����д��
   while(i) //1000 1ms    30 =0.03ms =30us    
   {       
     i--; 
   }  
}

void delay5ms(void)  //5ms��ʱ
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}


//iic��ʼ
bool I2C_Start(void) //������ʼ�ź� SCL�� SDA�½���
{
	SDA_H;  
	SCL_H;           //SDA SCL���Ǹߵ�ƽ�ȴ�ģʽ
	I2C_delay();
	if(!SDA_read)return FALSE;	//�������SDAΪ0 ���˳�(��ֹ��ʼʱSDAΪ1)
	SDA_L;               //��SDA��0 SDA�½��ؿ�ʼ��
	I2C_delay();
	if(SDA_read) return FALSE;	//�������SDAΪ1 ���˳�(��ֹSDAû������)
	SDA_L;                                 
	I2C_delay();
	return TRUE;
}


//iicֹͣ
void I2C_Stop(void)  //����ֹͣ�ź� SCL�� SAD������
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 


//�ھŸ��ߵ�ƽ����Ӧ���ź�
void I2C_Ack(void) //����Ӧ���źŵĵ�ƽ��� SCL�͸ߵ��ڼ� SDAһֱ�� 
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 


//�ھŸ��ߵ�ƽû�в���Ӧ���ź�
void I2C_NoAck(void)  //������Ӧ���źŵĵ�ƽ��� SCL�͸ߵ��ڼ� SDAһֱΪ�� 
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 



//��Ƭ����Ӧ���ź�
//����ֵ: 0ΪӦ��ʧ��
//        1ΪӦ��ɹ�
bool I2C_WaitAck(void) 	  //����Ӧ���ڼ�SCL�͸ߵ�   SDA�ߵ�
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)   //SDA���Ǹ� û�б�ͣ�Ӧ��ʧ��
	{
      SCL_L;     //SCL���
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;  //true=1
}




//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//   1 ��Ӧ��
//   2 ��Ӧ��
void I2C_SendByte(u8 SendByte) //���ݴӸߵ���  ����������SDA�ߵ�ʱ���۴���SCL�ߵ�ʱ�� 
{
    u8 i=8;
    while(i--)  //8λ=1�ֽ����� ����8��
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80) //���͵��������λ��1
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;  //���ƣ��θ�λ���λ
        I2C_delay();
		SCL_H;             
        I2C_delay();
    }
    SCL_L;
}  



//��ȡһ���ֽ�����
unsigned char I2C_RadeByte(void)  //���ݴӸ�λ����λ�� ������SCL�ߵ�ƽʱ��Ҫ����SDAʱ��
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;   //�õ�һ��������һλ   
      SCL_L;
      I2C_delay();
	    SCL_H;
      I2C_delay();	
      if(SDA_read)      //SDA�����ߵ�ƽ
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 


//iic���ֽ�д�뺯��(�������оƬ��) slave address������ַ  REG_Address�Ĵ�����ַ�    REG_data�Ĵ���д��ֵ
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�(0)
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //���ͼĴ�����ַ     
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}



//iic���ֽڶ�ȡ����(�����оƬ��)   slave address�ⲿоƬ��ַ   REG_Address���豸�ļĴ�����ַ
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data; //��Ŷ�����������    	
	if(!I2C_Start())return FALSE; //���I2C�Ƿ�ʼ
    I2C_SendByte(SlaveAddress); //�����豸��ַ+д�ź�(0)
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;} //���I2C�Ƿ���Ӧ��
    I2C_SendByte((u8) REG_Address);   //���ͼĴ�����ַ   
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1); //�����豸��ַ+���ź�(1)
    I2C_WaitAck();

	  REG_data= I2C_RadeByte(); 
    I2C_NoAck();  //������Ӧ���źţ��Ѷ��������������ȴ�������������������
    I2C_Stop();                                  //������Ӧ���źŽ��ճɹ� �ּ�������������
    //return TRUE;
	return REG_data; //���ض���������

}						      
	


//��ֵת������ 
void DATA_printf(uchar *s,short temp_data)  //��temp_data��ֵ ��λ ����s����Ϊ������ֵ����Ϊ��
{
	if(temp_data<0)   //���������ֵΪ���������ȸ� s ��ֵһ������
		{
	   temp_data=-temp_data;
     *s='-';
	  }
	else *s='+';     //����ֵ����
    *++s =temp_data/100+0x30;     //ȡ��λ        asc��0x30��0 temp_data%100�Ͱ�temp_data��Ϊֻ��ʮλ�Ժ�
    temp_data=temp_data%100;     
    *++s =temp_data/10+0x30;      //ȡʮλ
    temp_data=temp_data%10;      
    *++s =temp_data+0x30; 	      //ȡ��λ
}
//�����ż���λ���ݷ���ָ��s  sָ������TX_DATA  �ٰ�����TX_DATA���ĸ����ݰ�λ����

