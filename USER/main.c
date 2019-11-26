#include  <math.h> 
#include "stm32f10x.h"
#include "mpu9250.h"
#include "myiic.h"
#include "usart.h"
#include "niming.h"
#include "led.h"

/******************
PB6--SCL使能时序端    PB7-----SDA信号传输段
*******************/


int main(void)
{ 
  //RCC_Configuration();		 //配置RCC
  GPIO_Configuration();		  //串口通信引脚 初始化
  USART1_Configuration();	  //串口1初始化
  I2C_GPIO_Config();		    //IIC端口初始化
  Delayms(10);				      //延时
  Init_MPU9250();		        //初始化MPU9250
 while(1)
 {
	 
	 READ_MPU9250_ACCEL(); 
	 READ_MPU9250_GYRO();
	 READ_MPU9250_MAG();
	 

	 data_to_computer(ACC_X,ACC_Y,ACC_Z, GYR_X,GYR_Y,GYR_Z, MAG_X,MAG_Y,MAG_Z);
	 
	 LED0 =!LED0;
	 Delayms(10);	

	 
	 
	 
//	READ_MPU9250_ACCEL();  //加速度
//	DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
//	Send_data('A','X');			 //发送X轴数
//	DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
//	Send_data('A','Y');			 //发送Y轴数
//	DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
//	Send_data('A','Z');			 //发送Z轴数
//	 
//	READ_MPU9250_GYRO();      //陀螺
//	DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
//	Send_data('G','X');			 //发送X轴数
//	DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
//	Send_data('G','Y');			 //发送Y轴数
//	DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
//	Send_data('G','Z');			 //发送Z轴数
//	 
//	READ_MPU9250_MAG();	      //磁场
//  DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
//	Send_data('M','X');			 //发送X轴数
//	DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
//	Send_data('M','Y');			 //发送Y轴数
//	DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
//	Send_data('M','Z');			 //发送Z轴数
//	
//	/*
//	DATA_printf(TX_DATA,T_T);//转换温度数据到数组
//	Send_data('T');			 //发送温度数据
//	*/
//	
//	USART1_SendData(0X0D);	 //换行
//	USART1_SendData(0X0A);	 //回车
//	Delayms(10);				 //延时
  }
}

