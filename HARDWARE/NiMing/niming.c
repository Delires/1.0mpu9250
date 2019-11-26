#include "niming.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))     
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))   
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 data_to_send[50] ; //缓存数据

void data_to_computer(u16 data1, u16 data2, u16 data3, u16 data4, u16 data5, u16 data6, u16 data7, u16 data8, u16 data9)
{
    
    u8 i = 0;
    u8 cnt = 0;
    u8 sum = 0;
    
    data_to_send[cnt++]=0xAA;     //帧头0xAA不变
    data_to_send[cnt++]=0x05;     //发送源(不同片子不一样) 都选择0x05
    data_to_send[cnt++]=0xAF;     //发送目的地0xAF代表上位机
    data_to_send[cnt++]=0X02;    //此位相当于选择上位机功能  F1高级收码表  02传感器数据+高级收码
    data_to_send[cnt++]=0;   //数据长度     

    data_to_send[cnt++] = BYTE1(data1);
    data_to_send[cnt++] = BYTE0(data1);
		
    data_to_send[cnt++] = BYTE1(data2);
    data_to_send[cnt++] = BYTE0(data2);
		
		data_to_send[cnt++] = BYTE1(data3);
    data_to_send[cnt++] = BYTE0(data3);
	
	  data_to_send[cnt++] = BYTE1(data4);
    data_to_send[cnt++] = BYTE0(data4);
		
    data_to_send[cnt++] = BYTE1(data5);
    data_to_send[cnt++] = BYTE0(data5);
		
		data_to_send[cnt++] = BYTE1(data6);
    data_to_send[cnt++] = BYTE0(data6);
		
		data_to_send[cnt++] = BYTE1(data7);
    data_to_send[cnt++] = BYTE0(data7);
		
    data_to_send[cnt++] = BYTE1(data8);
    data_to_send[cnt++] = BYTE0(data8);
		
		data_to_send[cnt++] = BYTE1(data9);
    data_to_send[cnt++] = BYTE0(data9);


    data_to_send[4] = cnt-5;

    for(i=0;i<cnt;i++)
    {
        sum += data_to_send[i];
    }
    data_to_send[cnt++] = sum;   
		
		//usart1_Sendstr(data_to_send);
    send_line_data(data_to_send, cnt);

}
