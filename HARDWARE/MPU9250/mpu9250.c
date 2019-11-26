#include "mpu9250.h"
#include "myiic.h"
#include "usart.h"
#include "delay.h"


//�궨���������ͣ��ѼĴ�����λ�ú궨������

                      /***********����MPU9250�ڲ���ַ***************/

#define	SMPLRT_DIV		0x19    	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	      //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	    //�������Լ켰������Χ0x18  ����2000��
#define	ACCEL_CONFIG	0x1C	    //���ټ��Լ졢������Χ0x18  ����16g
#define ACCEL_CONFIG2 0x1D       //���ٶȼƵ�ͨ�˲��� 0x06(5Hz)
#define PWR_MGMT_1    0x6B   //��Դ����1 0x00
#define PWR_MGMT_2    0x6C   //��Դ����2 0x00

#define	WHO_AM_I		  0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define I2C_ADDR      0X68  //i2c��ַ
#define USER_CTRL     0x6A  //�û�����Ϊ0X10ʱΪSPIģʽ

#define MPU9250_CS    PDout(3) //MPU9250Ƭѡ�ź�


#define	ACCEL_XOUT_H	0x3B   //���ٶȼ�������ݣ��ɸߵ�16λ���
#define	ACCEL_XOUT_L	0x3C   //xyz������
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //�¶�������ݣ��ߵ���λ
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43   //������������� �ߵ���λ���
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03   //�������������
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08




//������ ������  ���ٶȼƵ�ַ
#define	GYRO_ADDRESS   0xD0	  //�����ǵ�ַ
#define MAG_ADDRESS    0x18   //�����Ƶ�ַ
#define ACCEL_ADDRESS  0xD0   //���ٶȼƵ�ַ


         //ErrorStatus(����ö�ٱ���)  ����ΪHSEStartUpStatus,���ķ���ֵֻ��0��1 error��success
 u8 TX_DATA[4];  	 //��ʾ�ݻ�����
 u8 BUF[10];       //�������ݻ�����
 
 short ACC_X,ACC_Y,ACC_Z;		 //X,Y,Z����ٶ�
 short GYR_X,GYR_Y,GYR_Z;		 //X,Y,Z����ٶ�
 short MAG_X,MAG_Y,MAG_Z;		 //X,Y,Z������ֵ
 



//��ʼ��MPU9250��������Ҫ��ο�pdf�����޸�************************
void Init_MPU9250(void)  //����mpu9250   
{
//  ��IIC�ĵ��ֽں��� ��mpu9250д����  �����ѻ����9250
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //��2000��
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  Single_Write(I2C_ADDR,PWR_MGMT_1, 0x00);	//�������״̬
	Single_Write(I2C_ADDR,SMPLRT_DIV, 0x07);
	Single_Write(I2C_ADDR,CONFIG, 0x06);        //��IIC��д���ֽں��� Single_Write()
	
//���� acc gyro ����������
	Single_Write(I2C_ADDR,GYRO_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG2, 0x00);   //ͨ��I2C ��mpu�Ĵ���д���ܻ���
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}






//******��ȡMPU9250����   ���ٶȼ�   ����  ������*************************
void READ_MPU9250_ACCEL(void)   //�����ٶȼƵ�ֵ
{ 
//*****************************��IIC�Ķ����ֽں����������ٶȼƵ�ֵ*****************/ 
//��ȡx����ļ��ٶ�
   BUF[0]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_L);   //IIC��ȡ ���ٵĵĵ�8λ
   BUF[1]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_H);    //IIC��ȡ ���ٵĵĸ
   ACC_X=	((u16)BUF[1]<<8)|BUF[0];
   ACC_X/=16.4; 						   

//��ȡy����ļ��ٶ�	
   BUF[2]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_L);    //Single_Read()
   BUF[3]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_H);
   ACC_Y=	((u16)BUF[3]<<8)|BUF[2]; //�ߵ�λ�����һ��
   ACC_Y/=16.4;
	
//��ȡz����ļ��ٶ�
   BUF[4]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   ACC_Z=	((u16)BUF[5]<<8)|BUF[4]; //�ߵ�λһ��
   ACC_Z/=16.4; 					       //��ȡ����Z������
	
	
	//���ٶ�ΪɶҪ��164 ���̳�16.4  �����Ʋ���
 
}



void READ_MPU9250_GYRO(void)   //�����̵�ֵ
{ 
////*****************************��IIC�Ķ����ֽں����������̵�ֵ****************/ 
   BUF[0]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_H);
   GYR_X=	((u16)BUF[1]<<8)|BUF[0];
   GYR_X/=16.4; 						   //��ȡ����X������

   BUF[2]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_H);
   GYR_Y=	((u16)BUF[3]<<8)|BUF[2];
   GYR_Y/=16.4; 						   //��ȡ����Y������
	
   BUF[4]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_H);
   GYR_Z=	((u16)BUF[5]<<8)|BUF[4];
   GYR_Z/=16.4; 					       //��ȡ����Z������
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
}



void READ_MPU9250_MAG(void)    //��ȡ�����Ƶ�ֵ
{ 
   Single_Write(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode ��·ģʽ ����������ͷ
	
//��SysTick��ʱ��д��ʱ �ͻ������ȡ���ݵ�����
//ԭ��Ϊ    iicͨ��ʱ  ��ΪSysTick��ʱ���ж�  ����iicͨ��
	 Delayms(10);	
	
   Single_Write(MAG_ADDRESS,0x0A,0x01);  //�Ĵ���дֵ�򿪴�����
	              // MAG_ADDRESS 0x18       0x01�������Լ�
	
//��SysTick��ʱ��д��ʱ �ͻ������ȡ���ݵ�����	
 	 Delayms(10);
	 BUF[0]=Single_Read (MAG_ADDRESS,MAG_XOUT_L);   
   BUF[1]=Single_Read (MAG_ADDRESS,MAG_XOUT_H);
   MAG_X=((u16)BUF[1]<<8)|BUF[0];

   BUF[2]=Single_Read(MAG_ADDRESS,MAG_YOUT_L);
   BUF[3]=Single_Read(MAG_ADDRESS,MAG_YOUT_H);
   MAG_Y=	((u16)BUF[3]<<8)|BUF[2]; 
   						   //��ȡ����Y������
	 
   BUF[4]=Single_Read(MAG_ADDRESS,MAG_ZOUT_L);
   BUF[5]=Single_Read(MAG_ADDRESS,MAG_ZOUT_H);
   MAG_Z=	((u16)BUF[5]<<8)|BUF[4];
 					       //��ȡ����Z������

}




 //********���ڷ������ݺ���***************************************
 void Send_data(uchar MAG,uchar axis)
 {uchar i;
	USART1_SendData(MAG);  //�����ַ�1   ��A
  USART1_SendData(axis); //�����ַ�2   ��X
  USART1_SendData(':');  //����ð��
 
  for(i=0;i<4;i++)
	{
	  USART1_SendData(TX_DATA[i]);  //DATA_printf(TX_DATA,T_X); 
  }      //��T_X������ֵ����ֵ��TX_DATA �ٰ����ݰ�λ��������

	USART1_SendData(' ');
  USART1_SendData(' ');
 }


