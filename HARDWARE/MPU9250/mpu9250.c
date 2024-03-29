#include "mpu9250.h"
#include "myiic.h"
#include "usart.h"
#include "delay.h"


//宏定义数据类型，把寄存器的位置宏定义成语句

                      /***********定义MPU9250内部地址***************/

#define	SMPLRT_DIV		0x19    	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	      //低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	    //陀螺仪自检及测量范围0x18  正负2000度
#define	ACCEL_CONFIG	0x1C	    //加速计自检、测量范围0x18  正负16g
#define ACCEL_CONFIG2 0x1D       //加速度计低通滤波器 0x06(5Hz)
#define PWR_MGMT_1    0x6B   //电源管理1 0x00
#define PWR_MGMT_2    0x6C   //电源管理2 0x00

#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x68，只读)
#define I2C_ADDR      0X68  //i2c地址
#define USER_CTRL     0x6A  //用户配置为0X10时为SPI模式

#define MPU9250_CS    PDout(3) //MPU9250片选信号


#define	ACCEL_XOUT_H	0x3B   //加速度计输出数据，由高低16位组成
#define	ACCEL_XOUT_L	0x3C   //xyz三方向
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //温度输出数据，高低两位
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43   //陀螺仪输出数据 高低两位组成
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03   //磁力计输出数据
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08




//陀螺仪 磁力计  加速度计地址
#define	GYRO_ADDRESS   0xD0	  //陀螺仪地址
#define MAG_ADDRESS    0x18   //磁力计地址
#define ACCEL_ADDRESS  0xD0   //加速度计地址


         //ErrorStatus(定义枚举变量)  变量为HSEStartUpStatus,它的返回值只有0和1 error和success
 u8 TX_DATA[4];  	 //显示据缓存区
 u8 BUF[10];       //接收数据缓存区
 
 short ACC_X,ACC_Y,ACC_Z;		 //X,Y,Z轴加速度
 short GYR_X,GYR_Y,GYR_Z;		 //X,Y,Z轴角速度
 short MAG_X,MAG_Y,MAG_Z;		 //X,Y,Z轴罗盘值
 



//初始化MPU9250，根据需要请参考pdf进行修改************************
void Init_MPU9250(void)  //配置mpu9250   
{
//  用IIC的单字节函数 向mpu9250写数据  来唤醒或控制9250
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  Single_Write(I2C_ADDR,PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_Write(I2C_ADDR,SMPLRT_DIV, 0x07);
	Single_Write(I2C_ADDR,CONFIG, 0x06);        //用IIC的写单字节函数 Single_Write()
	
//设置 acc gyro 磁力计量程
	Single_Write(I2C_ADDR,GYRO_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG2, 0x00);   //通过I2C 给mpu寄存器写来总唤醒
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}






//******读取MPU9250数据   加速度计   罗盘  磁力计*************************
void READ_MPU9250_ACCEL(void)   //读加速度计的值
{ 
//*****************************用IIC的读单字节函数，读加速度计的值*****************/ 
//读取x方向的加速度
   BUF[0]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_L);   //IIC读取 加速的的低8位
   BUF[1]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_H);    //IIC读取 加速的的�
   ACC_X=	((u16)BUF[1]<<8)|BUF[0];
   ACC_X/=16.4; 						   

//读取y方向的加速度	
   BUF[2]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_L);    //Single_Read()
   BUF[3]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_H);
   ACC_Y=	((u16)BUF[3]<<8)|BUF[2]; //高低位结合在一起
   ACC_Y/=16.4;
	
//读取z方向的加速度
   BUF[4]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   ACC_Z=	((u16)BUF[5]<<8)|BUF[4]; //高低位一与
   ACC_Z/=16.4; 					       //读取计算Z轴数据
	
	
	//加速度为啥要除164 罗盘除16.4  磁力计不除
 
}



void READ_MPU9250_GYRO(void)   //读罗盘的值
{ 
////*****************************用IIC的读单字节函数，读罗盘的值****************/ 
   BUF[0]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_H);
   GYR_X=	((u16)BUF[1]<<8)|BUF[0];
   GYR_X/=16.4; 						   //读取计算X轴数据

   BUF[2]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_H);
   GYR_Y=	((u16)BUF[3]<<8)|BUF[2];
   GYR_Y/=16.4; 						   //读取计算Y轴数据
	
   BUF[4]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_H);
   GYR_Z=	((u16)BUF[5]<<8)|BUF[4];
   GYR_Z/=16.4; 					       //读取计算Z轴数据
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}



void READ_MPU9250_MAG(void)    //读取磁力计的值
{ 
   Single_Write(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 旁路模式 磁力计在里头
	
//用SysTick定时器写延时 就会产生读取数据的问题
//原因为    iic通信时  因为SysTick延时有中断  会打断iic通信
	 Delayms(10);	
	
   Single_Write(MAG_ADDRESS,0x0A,0x01);  //寄存器写值打开磁力计
	              // MAG_ADDRESS 0x18       0x01磁力计自检
	
//用SysTick定时器写延时 就会产生读取数据的问题	
 	 Delayms(10);
	 BUF[0]=Single_Read (MAG_ADDRESS,MAG_XOUT_L);   
   BUF[1]=Single_Read (MAG_ADDRESS,MAG_XOUT_H);
   MAG_X=((u16)BUF[1]<<8)|BUF[0];

   BUF[2]=Single_Read(MAG_ADDRESS,MAG_YOUT_L);
   BUF[3]=Single_Read(MAG_ADDRESS,MAG_YOUT_H);
   MAG_Y=	((u16)BUF[3]<<8)|BUF[2]; 
   						   //读取计算Y轴数据
	 
   BUF[4]=Single_Read(MAG_ADDRESS,MAG_ZOUT_L);
   BUF[5]=Single_Read(MAG_ADDRESS,MAG_ZOUT_H);
   MAG_Z=	((u16)BUF[5]<<8)|BUF[4];
 					       //读取计算Z轴数据

}




 //********串口发送数据函数***************************************
 void Send_data(uchar MAG,uchar axis)
 {uchar i;
	USART1_SendData(MAG);  //发送字符1   如A
  USART1_SendData(axis); //发送字符2   如X
  USART1_SendData(':');  //发送冒号
 
  for(i=0;i<4;i++)
	{
	  USART1_SendData(TX_DATA[i]);  //DATA_printf(TX_DATA,T_X); 
  }      //把T_X读出的值按赋值给TX_DATA 再把数据按位发给串口

	USART1_SendData(' ');
  USART1_SendData(' ');
 }


