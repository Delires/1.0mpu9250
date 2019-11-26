#include "niming.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))     
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))   
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 data_to_send[50] ; //��������

void data_to_computer(u16 data1, u16 data2, u16 data3, u16 data4, u16 data5, u16 data6, u16 data7, u16 data8, u16 data9)
{
    
    u8 i = 0;
    u8 cnt = 0;
    u8 sum = 0;
    
    data_to_send[cnt++]=0xAA;     //֡ͷ0xAA����
    data_to_send[cnt++]=0x05;     //����Դ(��ͬƬ�Ӳ�һ��) ��ѡ��0x05
    data_to_send[cnt++]=0xAF;     //����Ŀ�ĵ�0xAF������λ��
    data_to_send[cnt++]=0X02;    //��λ�൱��ѡ����λ������  F1�߼������  02����������+�߼�����
    data_to_send[cnt++]=0;   //���ݳ���     

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
