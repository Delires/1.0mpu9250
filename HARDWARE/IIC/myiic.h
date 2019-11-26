#ifndef __myiic_h__
#define __myiic_h__

#include "stm32f10x.h"
#include "mpu9250.h"
 

typedef enum {FALSE = 0,TRUE = 1} bool;
         //bool  false定义
extern char  test; 	
extern ErrorStatus HSEStartUpStatus;



void I2C_GPIO_Config(void);  //配置PB6为SCL使能时序端    PB7为SDA信号传输段
bool I2C_Start(void); //发送起始信号 SCL高 SDA下降沿
void I2C_Stop(void);  //发送停止信号 SCL高 SAD上升沿

void I2C_Ack(void);     //产生应答信号的电平情况 SCL低高低期间 SDA一直低 	
void I2C_NoAck(void);   //不产生应答信号的电平情况 SCL低高低期间 SDA一直为高 
bool I2C_WaitAck(void);  //产生应答期间SCL低高低   SDA高低

void I2C_SendByte(u8 SendByte);    //发送一个字节数据  发送数据是SDA高的时间眼大于SCL高的时间 
unsigned char I2C_RadeByte(void);  //读取一个字节数据  读数据SCL高电平时间要大于SDA时间


//iic单字节写入函数(用于向MPU9250写数据，用来设置MPU9250)
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
//iic单字节读取函数
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);


void DATA_printf(uchar *s,short temp_data);  //数值转换函数


void delay5ms(void);  //5ms延时
void I2C_delay(void);   //I2C的延时函数

#endif









