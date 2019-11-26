/**
��Ӳ����֪ PA9--UASTR1_TX    PA10--UASTR1_RX

����ͨ�����ò���
1.����ʱ��ʹ��GPIO                         R9CC_APB2PeriphClockCmd();
  ʹ��USART1(ʹ�ܵڶ����ܼ��˿ڸ�����)     RCC_APB2PeriphClockCmd();
2.���ڸ�λ(�Ǳ���)                         USART_DeInit();
3.GPIO�˿�ģʽ����                         GPIO_Init();   �����Ĳο��ֲ�8.1.11
4.���ڲ�����ʼ��(����Ҫ��ʹ��ʱ������ͬ)   USART_Init();
5.����ʹ�ܺ���                             USART_Cmd();
6.�����ж�
	6.2���������ж�                          USART_ITConfig();
  6.1�����жϷ���(��������д)              NVIC_PriorityGroupConfig();
	6.3�����ж�(�������ȼ���ʹ�ܣ��жϺ�)    NVIC_Init();
	6.4д�жϷ�����(core-startup)          USARTx_IRQHandler();
7.���������շ�
           ���� USART_SendData();
           ��ȡ USART_ReceiveData();					 
**/

#include "usart.h"


//��д֧��printf�����Ĵ��룬���þ��С���USART���ݱ�Ÿ�
#if 1     
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif


int rec;




//�˿�����
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE  );
   /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //	ѡ�йܽ�9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // ����������50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);				 // ѡ��A�˿�
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  //ѡ�йܽ�10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);				  //ѡ��A�˿�

}


//����ͨ������
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_USART1, ENABLE  );
		
	USART_InitStructure.USART_BaudRate = 500000;						  // ������Ϊ��115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // ��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // ��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ��������ʧ��

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // ����ʹ��+����ʹ��
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1, &USART_InitStructure);
			
		/* Enable USART1 */
	USART_ClearFlag(USART1, USART_IT_RXNE); 			//���жϣ�����һ�����жϺ����������ж�
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//ʹ��USART1�ж�Դ
	USART_Cmd(USART1, ENABLE);							//USART1�ܿ��أ����� 
}

/****************************��ʱ����**********************************/
/************������������ʱ  �����Դ��Ķ�ʱ������ʱ���� �������ݻῨ��************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}




//��λ�������ݺ���
void  USART1_SendData(u8 SendData)
{
	while( (USART_GetFlagStatus(USART1, USART_FLAG_TXE)) != SET);  //��黺�����ڴ��Ƿ�Ϊ��,������ϸ������Ѿ����ͳ�ȥ��
  USART_SendData(USART1, SendData);                               //�ͷ�����һ���������Ϊ�ա��Ϳ����������Ϊ���ٷ��� RESET 0 SET=1
}


//�����ַ�������
void USART1_Send_string(u8 *S)
{
	while(*S)
	{
		USART1_SendData(*S++);
	}
}


//�����ȷ��ͺ���
void send_line_data(u8 *s, u8 line)
{
	u8 i;
	for(i=0; i<line; i++)
	{
		USART1_SendData(*s++);
	}
}

