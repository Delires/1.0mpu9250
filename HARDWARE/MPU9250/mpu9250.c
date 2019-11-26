#include "mpu9250.h"
#include "myiic.h"
#include "usart.h"
#include "delay.h"


//ºê¶¨ÒåÊı¾İÀàĞÍ£¬°Ñ¼Ä´æÆ÷µÄÎ»ÖÃºê¶¨Òå³ÉÓï¾ä

                      /***********¶¨ÒåMPU9250ÄÚ²¿µØÖ·***************/

#define	SMPLRT_DIV		0x19    	//ÍÓÂİÒÇ²ÉÑùÂÊ£¬µäĞÍÖµ£º0x07(125Hz)
#define	CONFIG			0x1A	      //µÍÍ¨ÂË²¨ÆµÂÊ£¬µäĞÍÖµ£º0x06(5Hz)
#define	GYRO_CONFIG		0x1B	    //ÍÓÂİÒÇ×Ô¼ì¼°²âÁ¿·¶Î§0x18  Õı¸º2000¶È
#define	ACCEL_CONFIG	0x1C	    //¼ÓËÙ¼Æ×Ô¼ì¡¢²âÁ¿·¶Î§0x18  Õı¸º16g
#define ACCEL_CONFIG2 0x1D       //¼ÓËÙ¶È¼ÆµÍÍ¨ÂË²¨Æ÷ 0x06(5Hz)
#define PWR_MGMT_1    0x6B   //µçÔ´¹ÜÀí1 0x00
#define PWR_MGMT_2    0x6C   //µçÔ´¹ÜÀí2 0x00

#define	WHO_AM_I		  0x75	//IICµØÖ·¼Ä´æÆ÷(Ä¬ÈÏÊıÖµ0x68£¬Ö»¶Á)
#define I2C_ADDR      0X68  //i2cµØÖ·
#define USER_CTRL     0x6A  //ÓÃ»§ÅäÖÃÎª0X10Ê±ÎªSPIÄ£Ê½

#define MPU9250_CS    PDout(3) //MPU9250Æ¬Ñ¡ĞÅºÅ


#define	ACCEL_XOUT_H	0x3B   //¼ÓËÙ¶È¼ÆÊä³öÊı¾İ£¬ÓÉ¸ßµÍ16Î»×é³É
#define	ACCEL_XOUT_L	0x3C   //xyzÈı·½Ïò
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //ÎÂ¶ÈÊä³öÊı¾İ£¬¸ßµÍÁ½Î»
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43   //ÍÓÂİÒÇÊä³öÊı¾İ ¸ßµÍÁ½Î»×é³É
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03   //´ÅÁ¦¼ÆÊä³öÊı¾İ
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08




//ÍÓÂİÒÇ ´ÅÁ¦¼Æ  ¼ÓËÙ¶È¼ÆµØÖ·
#define	GYRO_ADDRESS   0xD0	  //ÍÓÂİÒÇµØÖ·
#define MAG_ADDRESS    0x18   //´ÅÁ¦¼ÆµØÖ·
#define ACCEL_ADDRESS  0xD0   //¼ÓËÙ¶È¼ÆµØÖ·


         //ErrorStatus(¶¨ÒåÃ¶¾Ù±äÁ¿)  ±äÁ¿ÎªHSEStartUpStatus,ËüµÄ·µ»ØÖµÖ»ÓĞ0ºÍ1 errorºÍsuccess
 u8 TX_DATA[4];  	 //ÏÔÊ¾¾İ»º´æÇø
 u8 BUF[10];       //½ÓÊÕÊı¾İ»º´æÇø
 
 short ACC_X,ACC_Y,ACC_Z;		 //X,Y,ZÖá¼ÓËÙ¶È
 short GYR_X,GYR_Y,GYR_Z;		 //X,Y,ZÖá½ÇËÙ¶È
 short MAG_X,MAG_Y,MAG_Z;		 //X,Y,ZÖáÂŞÅÌÖµ
 



//³õÊ¼»¯MPU9250£¬¸ù¾İĞèÒªÇë²Î¿¼pdf½øĞĞĞŞ¸Ä************************
void Init_MPU9250(void)  //ÅäÖÃmpu9250   
{
//  ÓÃIICµÄµ¥×Ö½Úº¯Êı Ïòmpu9250Ğ´Êı¾İ  À´»½ĞÑ»ò¿ØÖÆ9250
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //¡À2000¡ã
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  Single_Write(I2C_ADDR,PWR_MGMT_1, 0x00);	//½â³ıĞİÃß×´Ì¬
	Single_Write(I2C_ADDR,SMPLRT_DIV, 0x07);
	Single_Write(I2C_ADDR,CONFIG, 0x06);        //ÓÃIICµÄĞ´µ¥×Ö½Úº¯Êı Single_Write()
	
//ÉèÖÃ acc gyro ´ÅÁ¦¼ÆÁ¿³Ì
	Single_Write(I2C_ADDR,GYRO_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG, 0x18);
	Single_Write(I2C_ADDR,ACCEL_CONFIG2, 0x00);   //Í¨¹ıI2C ¸ømpu¼Ä´æÆ÷Ğ´À´×Ü»½ĞÑ
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}






//******¶ÁÈ¡MPU9250Êı¾İ   ¼ÓËÙ¶È¼Æ   ÂŞÅÌ  ´ÅÁ¦¼Æ*************************
void READ_MPU9250_ACCEL(void)   //¶Á¼ÓËÙ¶È¼ÆµÄÖµ
{ 
//*****************************ÓÃIICµÄ¶Áµ¥×Ö½Úº¯Êı£¬¶Á¼ÓËÙ¶È¼ÆµÄÖµ*****************/ 
//¶ÁÈ¡x·½ÏòµÄ¼ÓËÙ¶È
   BUF[0]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_L);   //IIC¶ÁÈ¡ ¼ÓËÙµÄµÄµÍ8Î»
   BUF[1]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_H);    //IIC¶ÁÈ¡ ¼ÓËÙµÄµÄ¸
   ACC_X=	((u16)BUF[1]<<8)|BUF[0];
   ACC_X/=16.4; 						   

//¶ÁÈ¡y·½ÏòµÄ¼ÓËÙ¶È	
   BUF[2]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_L);    //Single_Read()
   BUF[3]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_H);
   ACC_Y=	((u16)BUF[3]<<8)|BUF[2]; //¸ßµÍÎ»½áºÏÔÚÒ»Æğ
   ACC_Y/=16.4;
	
//¶ÁÈ¡z·½ÏòµÄ¼ÓËÙ¶È
   BUF[4]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   ACC_Z=	((u16)BUF[5]<<8)|BUF[4]; //¸ßµÍÎ»Ò»Óë
   ACC_Z/=16.4; 					       //¶ÁÈ¡¼ÆËãZÖáÊı¾İ
	
	
	//¼ÓËÙ¶ÈÎªÉ¶Òª³ı164 ÂŞÅÌ³ı16.4  ´ÅÁ¦¼Æ²»³ı
 
}



void READ_MPU9250_GYRO(void)   //¶ÁÂŞÅÌµÄÖµ
{ 
////*****************************ÓÃIICµÄ¶Áµ¥×Ö½Úº¯Êı£¬¶ÁÂŞÅÌµÄÖµ****************/ 
   BUF[0]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_H);
   GYR_X=	((u16)BUF[1]<<8)|BUF[0];
   GYR_X/=16.4; 						   //¶ÁÈ¡¼ÆËãXÖáÊı¾İ

   BUF[2]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_H);
   GYR_Y=	((u16)BUF[3]<<8)|BUF[2];
   GYR_Y/=16.4; 						   //¶ÁÈ¡¼ÆËãYÖáÊı¾İ
	
   BUF[4]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_H);
   GYR_Z=	((u16)BUF[5]<<8)|BUF[4];
   GYR_Z/=16.4; 					       //¶ÁÈ¡¼ÆËãZÖáÊı¾İ
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// ¶ÁÈ¡¼ÆËã³öÎÂ¶È
}



void READ_MPU9250_MAG(void)    //¶ÁÈ¡´ÅÁ¦¼ÆµÄÖµ
{ 
   Single_Write(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode ÅÔÂ·Ä£Ê½ ´ÅÁ¦¼ÆÔÚÀïÍ·
	
//ÓÃSysTick¶¨Ê±Æ÷Ğ´ÑÓÊ± ¾Í»á²úÉú¶ÁÈ¡Êı¾İµÄÎÊÌâ
//Ô­ÒòÎª    iicÍ¨ĞÅÊ±  ÒòÎªSysTickÑÓÊ±ÓĞÖĞ¶Ï  »á´ò¶ÏiicÍ¨ĞÅ
	 Delayms(10);	
	
   Single_Write(MAG_ADDRESS,0x0A,0x01);  //¼Ä´æÆ÷Ğ´Öµ´ò¿ª´ÅÁ¦¼Æ
	              // MAG_ADDRESS 0x18       0x01´ÅÁ¦¼Æ×Ô¼ì
	
//ÓÃSysTick¶¨Ê±Æ÷Ğ´ÑÓÊ± ¾Í»á²úÉú¶ÁÈ¡Êı¾İµÄÎÊÌâ	
 	 Delayms(10);
	 BUF[0]=Single_Read (MAG_ADDRESS,MAG_XOUT_L);   
   BUF[1]=Single_Read (MAG_ADDRESS,MAG_XOUT_H);
   MAG_X=((u16)BUF[1]<<8)|BUF[0];

   BUF[2]=Single_Read(MAG_ADDRESS,MAG_YOUT_L);
   BUF[3]=Single_Read(MAG_ADDRESS,MAG_YOUT_H);
   MAG_Y=	((u16)BUF[3]<<8)|BUF[2]; 
   						   //¶ÁÈ¡¼ÆËãYÖáÊı¾İ
	 
   BUF[4]=Single_Read(MAG_ADDRESS,MAG_ZOUT_L);
   BUF[5]=Single_Read(MAG_ADDRESS,MAG_ZOUT_H);
   MAG_Z=	((u16)BUF[5]<<8)|BUF[4];
 					       //¶ÁÈ¡¼ÆËãZÖáÊı¾İ

}




 //********´®¿Ú·¢ËÍÊı¾İº¯Êı***************************************
 void Send_data(uchar MAG,uchar axis)
 {uchar i;
	USART1_SendData(MAG);  //·¢ËÍ×Ö·û1   ÈçA
  USART1_SendData(axis); //·¢ËÍ×Ö·û2   ÈçX
  USART1_SendData(':');  //·¢ËÍÃ°ºÅ
 
  for(i=0;i<4;i++)
	{
	  USART1_SendData(TX_DATA[i]);  //DATA_printf(TX_DATA,T_X); 
  }      //°ÑT_X¶Á³öµÄÖµ°´¸³Öµ¸øTX_DATA ÔÙ°ÑÊı¾İ°´Î»·¢¸ø´®¿Ú

	USART1_SendData(' ');
  USART1_SendData(' ');
 }


