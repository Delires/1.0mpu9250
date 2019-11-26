#ifndef __usart_h__
#define __usart_h__

#include "stdio.h"
#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"

#define   uchar unsigned char
#define   uint unsigned int	

//**********************端口 和 串口通信 分开配置*********************//

extern int rec;

void GPIO_Configuration(void);     //PA9 PA10端口配置

void USART1_Configuration(void);          //串口通信配置

void  USART1_SendData(uchar SendData); //发送数据函数
void USART1_Send_string(u8 *S);        //发送字符串函数
void send_line_data(u8 *s, u8 line);   //按长度发送函数



//不用定时器的 延时函数  自己写的
void Delayms(vu32 m);
void Delay(vu32 nCount);

#endif


