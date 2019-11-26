#ifndef __usart_h__
#define __usart_h__

#include "stdio.h"
#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"

#define   uchar unsigned char
#define   uint unsigned int	

//**********************�˿� �� ����ͨ�� �ֿ�����*********************//

extern int rec;

void GPIO_Configuration(void);     //PA9 PA10�˿�����

void USART1_Configuration(void);          //����ͨ������

void  USART1_SendData(uchar SendData); //�������ݺ���
void USART1_Send_string(u8 *S);        //�����ַ�������
void send_line_data(u8 *s, u8 line);   //�����ȷ��ͺ���



//���ö�ʱ���� ��ʱ����  �Լ�д��
void Delayms(vu32 m);
void Delay(vu32 nCount);

#endif


