#ifndef __mpu9250_h__
#define __mpu9250_h__

#include "stm32f10x.h"
#include "usart.h"
#include "sys.h"

extern u8 TX_DATA[4];  	 //显示据缓存区
extern u8 BUF[10];       //接收数据缓存区
extern short ACC_X,ACC_Y,ACC_Z;		 //X,Y,Z轴加速度
extern short GYR_X,GYR_Y,GYR_Z;		 //X,Y,Z轴角速度
extern short MAG_X,MAG_Y,MAG_Z;		 //X,Y,Z轴罗盘值
//变量在main.c中用  因此要在.h中声明为全局变量  .c中重新定义




void Init_MPU9250(void);  //配置mpu9250

//读取加速度   罗盘  磁力计  数值
void READ_MPU9250_ACCEL(void);  //加速度计
void READ_MPU9250_GYRO(void);   //读取罗盘
void READ_MPU9250_MAG(void);    //读取磁力计

//串口发送数据函数  由usart的串口数据函数组成
void Send_data(uchar MAG,uchar axis);


#endif

