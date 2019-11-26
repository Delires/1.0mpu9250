#include  <math.h> 
#include "stm32f10x.h"
#include "mpu9250.h"
#include "myiic.h"
#include "usart.h"
#include "niming.h"
#include "led.h"

/******************
PB6--SCLʹ��ʱ���    PB7-----SDA�źŴ����
*******************/


int main(void)
{ 
  //RCC_Configuration();		 //����RCC
  GPIO_Configuration();		  //����ͨ������ ��ʼ��
  USART1_Configuration();	  //����1��ʼ��
  I2C_GPIO_Config();		    //IIC�˿ڳ�ʼ��
  Delayms(10);				      //��ʱ
  Init_MPU9250();		        //��ʼ��MPU9250
 while(1)
 {
	 
	 READ_MPU9250_ACCEL(); 
	 READ_MPU9250_GYRO();
	 READ_MPU9250_MAG();
	 

	 data_to_computer(ACC_X,ACC_Y,ACC_Z, GYR_X,GYR_Y,GYR_Z, MAG_X,MAG_Y,MAG_Z);
	 
	 LED0 =!LED0;
	 Delayms(10);	

	 
	 
	 
//	READ_MPU9250_ACCEL();  //���ٶ�
//	DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
//	Send_data('A','X');			 //����X����
//	DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
//	Send_data('A','Y');			 //����Y����
//	DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
//	Send_data('A','Z');			 //����Z����
//	 
//	READ_MPU9250_GYRO();      //����
//	DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
//	Send_data('G','X');			 //����X����
//	DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
//	Send_data('G','Y');			 //����Y����
//	DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
//	Send_data('G','Z');			 //����Z����
//	 
//	READ_MPU9250_MAG();	      //�ų�
//  DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
//	Send_data('M','X');			 //����X����
//	DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
//	Send_data('M','Y');			 //����Y����
//	DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
//	Send_data('M','Z');			 //����Z����
//	
//	/*
//	DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
//	Send_data('T');			 //�����¶�����
//	*/
//	
//	USART1_SendData(0X0D);	 //����
//	USART1_SendData(0X0A);	 //�س�
//	Delayms(10);				 //��ʱ
  }
}

