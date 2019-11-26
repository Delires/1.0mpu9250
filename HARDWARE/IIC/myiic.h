#ifndef __myiic_h__
#define __myiic_h__

#include "stm32f10x.h"
#include "mpu9250.h"
 

typedef enum {FALSE = 0,TRUE = 1} bool;
         //bool  false����
extern char  test; 	
extern ErrorStatus HSEStartUpStatus;



void I2C_GPIO_Config(void);  //����PB6ΪSCLʹ��ʱ���    PB7ΪSDA�źŴ����
bool I2C_Start(void); //������ʼ�ź� SCL�� SDA�½���
void I2C_Stop(void);  //����ֹͣ�ź� SCL�� SAD������

void I2C_Ack(void);     //����Ӧ���źŵĵ�ƽ��� SCL�͸ߵ��ڼ� SDAһֱ�� 	
void I2C_NoAck(void);   //������Ӧ���źŵĵ�ƽ��� SCL�͸ߵ��ڼ� SDAһֱΪ�� 
bool I2C_WaitAck(void);  //����Ӧ���ڼ�SCL�͸ߵ�   SDA�ߵ�

void I2C_SendByte(u8 SendByte);    //����һ���ֽ�����  ����������SDA�ߵ�ʱ���۴���SCL�ߵ�ʱ�� 
unsigned char I2C_RadeByte(void);  //��ȡһ���ֽ�����  ������SCL�ߵ�ƽʱ��Ҫ����SDAʱ��


//iic���ֽ�д�뺯��(������MPU9250д���ݣ���������MPU9250)
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
//iic���ֽڶ�ȡ����
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);


void DATA_printf(uchar *s,short temp_data);  //��ֵת������


void delay5ms(void);  //5ms��ʱ
void I2C_delay(void);   //I2C����ʱ����

#endif









