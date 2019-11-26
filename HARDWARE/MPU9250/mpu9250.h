#ifndef __mpu9250_h__
#define __mpu9250_h__

#include "stm32f10x.h"
#include "usart.h"
#include "sys.h"

extern u8 TX_DATA[4];  	 //��ʾ�ݻ�����
extern u8 BUF[10];       //�������ݻ�����
extern short ACC_X,ACC_Y,ACC_Z;		 //X,Y,Z����ٶ�
extern short GYR_X,GYR_Y,GYR_Z;		 //X,Y,Z����ٶ�
extern short MAG_X,MAG_Y,MAG_Z;		 //X,Y,Z������ֵ
//������main.c����  ���Ҫ��.h������Ϊȫ�ֱ���  .c�����¶���




void Init_MPU9250(void);  //����mpu9250

//��ȡ���ٶ�   ����  ������  ��ֵ
void READ_MPU9250_ACCEL(void);  //���ٶȼ�
void READ_MPU9250_GYRO(void);   //��ȡ����
void READ_MPU9250_MAG(void);    //��ȡ������

//���ڷ������ݺ���  ��usart�Ĵ������ݺ������
void Send_data(uchar MAG,uchar axis);


#endif

