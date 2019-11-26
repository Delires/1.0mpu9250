/**
由硬件可知 PA9--UASTR1_TX    PA10--UASTR1_RX

串行通信配置步骤
1.串口时钟使能GPIO                         R9CC_APB2PeriphClockCmd();
  使能USART1(使能第二功能即端口复用了)     RCC_APB2PeriphClockCmd();
2.串口复位(非必需)                         USART_DeInit();
3.GPIO端口模式设置                         GPIO_Init();   查中文参考手册8.1.11
4.串口参数初始化(配置要和使用时设置相同)   USART_Init();
5.串口使能函数                             USART_Cmd();
6.配置中断
	6.2开启串口中断                          USART_ITConfig();
  6.1配置中断分组(主函数里写)              NVIC_PriorityGroupConfig();
	6.3配置中断(两个优先级，使能，中断号)    NVIC_Init();
	6.4写中断服务函数(core-startup)          USARTx_IRQHandler();
7.串口数据收发
           发送 USART_SendData();
           读取 USART_ReceiveData();					 
**/

#include "usart.h"


//编写支持printf函数的代码，会用就行。把USART根据标号改
#if 1     
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif


int rec;




//端口配置
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE  );
   /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //	选中管脚9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);				 // 选择A端口
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  //选中管脚10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);				  //选择A端口

}


//串口通信配置
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_USART1, ENABLE  );
		
	USART_InitStructure.USART_BaudRate = 500000;						  // 波特率为：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1, &USART_InitStructure);
			
		/* Enable USART1 */
	USART_ClearFlag(USART1, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//使能USART1中断源
	USART_Cmd(USART1, ENABLE);							//USART1总开关：开启 
}

/****************************延时函数**********************************/
/************不用这两个延时  而用自带的定时器的延时函数 发送数据会卡死************/
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




//按位发送数据函数
void  USART1_SendData(u8 SendData)
{
	while( (USART_GetFlagStatus(USART1, USART_FLAG_TXE)) != SET);  //检查缓存区内存是否为空,如果空上个数据已经发送出去了
  USART_SendData(USART1, SendData);                               //就发送下一个，如果不为空。就卡死在这里，等为空再发送 RESET 0 SET=1
}


//发送字符串函数
void USART1_Send_string(u8 *S)
{
	while(*S)
	{
		USART1_SendData(*S++);
	}
}


//按长度发送函数
void send_line_data(u8 *s, u8 line)
{
	u8 i;
	for(i=0; i<line; i++)
	{
		USART1_SendData(*s++);
	}
}

