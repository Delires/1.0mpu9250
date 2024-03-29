#include "myiic.h"

//ErrorStatus HSEStartUpStatus;  //就是一个枚举类型的结构体，在此函数没啥用。

char  test=0; 				 //IIC用到
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6  //控制BSRR寄存器，设置SCL高电平
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6  //SCL为低电平
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7  //SDA 高
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7  //SDA 低

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6  //读SCL PB6引脚电平
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7  //读SDA PB7引脚电平



//配置PB6为SCL使能时序端    PB7为SDA信号传输段
void I2C_GPIO_Config(void)  //配置PB6为SCL使能时序端    PB7为SDA信号传输段
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



void I2C_delay(void)   //I2C的延时函数
{
		
   u8 i=30; //这里可以优化速度	，经测试最低到5还能写入
   while(i) //1000 1ms    30 =0.03ms =30us    
   {       
     i--; 
   }  
}

void delay5ms(void)  //5ms延时
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}


//iic开始
bool I2C_Start(void) //发送起始信号 SCL高 SDA下降沿
{
	SDA_H;  
	SCL_H;           //SDA SCL都是高电平等待模式
	I2C_delay();
	if(!SDA_read)return FALSE;	//如果读到SDA为0 则退出(防止开始时SDA为1)
	SDA_L;               //把SDA置0 SDA下降沿开始读
	I2C_delay();
	if(SDA_read) return FALSE;	//如果读到SDA为1 则退出(防止SDA没降下来)
	SDA_L;                                 
	I2C_delay();
	return TRUE;
}


//iic停止
void I2C_Stop(void)  //发送停止信号 SCL高 SAD上升沿
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


//第九个高电平产生应答信号
void I2C_Ack(void) //产生应答信号的电平情况 SCL低高低期间 SDA一直低 
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


//第九个高电平没有产生应答信号
void I2C_NoAck(void)  //不产生应答信号的电平情况 SCL低高低期间 SDA一直为高 
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



//单片机读应答信号
//返回值: 0为应答失败
//        1为应答成功
bool I2C_WaitAck(void) 	  //产生应答期间SCL低高低   SDA高低
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)   //SDA还是高 没有变低，应答失败
	{
      SCL_L;     //SCL变低
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;  //true=1
}




//IIC发送一个字节
//返回从机有无应答
//   1 有应答
//   2 无应答
void I2C_SendByte(u8 SendByte) //数据从高到低  发送数据是SDA高的时间眼大于SCL高的时间 
{
    u8 i=8;
    while(i--)  //8位=1字节数据 发送8次
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80) //发送的数据最高位是1
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;  //左移，次高位变高位
        I2C_delay();
		SCL_H;             
        I2C_delay();
    }
    SCL_L;
}  



//读取一个字节数据
unsigned char I2C_RadeByte(void)  //数据从高位到低位， 读数据SCL高电平时间要大于SDA时间
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;   //得到一个数左移一位   
      SCL_L;
      I2C_delay();
	    SCL_H;
      I2C_delay();	
      if(SDA_read)      //SDA读到高电平
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 


//iic单字节写入函数(启动外接芯片用) slave address器件地址  REG_Address寄存器地址�    REG_data寄存器写入值
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号(0)
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //发送寄存器地址     
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}



//iic单字节读取函数(读外接芯片用)   slave address外部芯片地址   REG_Address从设备的寄存器地址
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data; //存放都出来的数据    	
	if(!I2C_Start())return FALSE; //检查I2C是否开始
    I2C_SendByte(SlaveAddress); //发送设备地址+写信号(0)
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;} //检查I2C是否有应答
    I2C_SendByte((u8) REG_Address);   //发送寄存器地址   
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1); //发送设备地址+读信号(1)
    I2C_WaitAck();

	  REG_data= I2C_RadeByte(); 
    I2C_NoAck();  //不产生应答信号，把读数据拉起。让他等待不让他继续传达数据
    I2C_Stop();                                  //否则又应答信号接收成功 又继续接收数据了
    //return TRUE;
	return REG_data; //返回读到的数据

}						      
	


//数值转换函数 
void DATA_printf(uchar *s,short temp_data)  //把temp_data的值 按位 赋给s，因为测量的值可能为负
{
	if(temp_data<0)   //如果测量的值为负数，就先给 s 赋值一个负号
		{
	   temp_data=-temp_data;
     *s='-';
	  }
	else *s='+';     //否则赋值正号
    *++s =temp_data/100+0x30;     //取百位        asc码0x30是0 temp_data%100就把temp_data变为只含十位以后
    temp_data=temp_data%100;     
    *++s =temp_data/10+0x30;      //取十位
    temp_data=temp_data%10;      
    *++s =temp_data+0x30; 	      //取个位
}
//正负号加三位数据发给指针s  s指向数组TX_DATA  再把数组TX_DATA的四个数据按位发送

