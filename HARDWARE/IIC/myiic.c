#include "myiic.h"

//ErrorStatus HSEStartUpStatus;  //¾ÍÊÇÒ»¸öÃ¶¾ÙÀàĞÍµÄ½á¹¹Ìå£¬ÔÚ´Ëº¯ÊıÃ»É¶ÓÃ¡£

char  test=0; 				 //IICÓÃµ½
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6  //¿ØÖÆBSRR¼Ä´æÆ÷£¬ÉèÖÃSCL¸ßµçÆ½
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6  //SCLÎªµÍµçÆ½
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7  //SDA ¸ß
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7  //SDA µÍ

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6  //¶ÁSCL PB6Òı½ÅµçÆ½
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7  //¶ÁSDA PB7Òı½ÅµçÆ½



//ÅäÖÃPB6ÎªSCLÊ¹ÄÜÊ±Ğò¶Ë    PB7ÎªSDAĞÅºÅ´«Êä¶Î
void I2C_GPIO_Config(void)  //ÅäÖÃPB6ÎªSCLÊ¹ÄÜÊ±Ğò¶Ë    PB7ÎªSDAĞÅºÅ´«Êä¶Î
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}



void I2C_delay(void)   //I2CµÄÑÓÊ±º¯Êı
{
		
   u8 i=30; //ÕâÀï¿ÉÒÔÓÅ»¯ËÙ¶È	£¬¾­²âÊÔ×îµÍµ½5»¹ÄÜĞ´Èë
   while(i) //1000 1ms    30 =0.03ms =30us    
   {       
     i--; 
   }  
}

void delay5ms(void)  //5msÑÓÊ±
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}


//iic¿ªÊ¼
bool I2C_Start(void) //·¢ËÍÆğÊ¼ĞÅºÅ SCL¸ß SDAÏÂ½µÑØ
{
	SDA_H;  
	SCL_H;           //SDA SCL¶¼ÊÇ¸ßµçÆ½µÈ´ıÄ£Ê½
	I2C_delay();
	if(!SDA_read)return FALSE;	//Èç¹û¶Áµ½SDAÎª0 ÔòÍË³ö(·ÀÖ¹¿ªÊ¼Ê±SDAÎª1)
	SDA_L;               //°ÑSDAÖÃ0 SDAÏÂ½µÑØ¿ªÊ¼¶Á
	I2C_delay();
	if(SDA_read) return FALSE;	//Èç¹û¶Áµ½SDAÎª1 ÔòÍË³ö(·ÀÖ¹SDAÃ»½µÏÂÀ´)
	SDA_L;                                 
	I2C_delay();
	return TRUE;
}


//iicÍ£Ö¹
void I2C_Stop(void)  //·¢ËÍÍ£Ö¹ĞÅºÅ SCL¸ß SADÉÏÉıÑØ
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 


//µÚ¾Å¸ö¸ßµçÆ½²úÉúÓ¦´ğĞÅºÅ
void I2C_Ack(void) //²úÉúÓ¦´ğĞÅºÅµÄµçÆ½Çé¿ö SCLµÍ¸ßµÍÆÚ¼ä SDAÒ»Ö±µÍ 
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 


//µÚ¾Å¸ö¸ßµçÆ½Ã»ÓĞ²úÉúÓ¦´ğĞÅºÅ
void I2C_NoAck(void)  //²»²úÉúÓ¦´ğĞÅºÅµÄµçÆ½Çé¿ö SCLµÍ¸ßµÍÆÚ¼ä SDAÒ»Ö±Îª¸ß 
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 



//µ¥Æ¬»ú¶ÁÓ¦´ğĞÅºÅ
//·µ»ØÖµ: 0ÎªÓ¦´ğÊ§°Ü
//        1ÎªÓ¦´ğ³É¹¦
bool I2C_WaitAck(void) 	  //²úÉúÓ¦´ğÆÚ¼äSCLµÍ¸ßµÍ   SDA¸ßµÍ
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)   //SDA»¹ÊÇ¸ß Ã»ÓĞ±äµÍ£¬Ó¦´ğÊ§°Ü
	{
      SCL_L;     //SCL±äµÍ
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;  //true=1
}




//IIC·¢ËÍÒ»¸ö×Ö½Ú
//·µ»Ø´Ó»úÓĞÎŞÓ¦´ğ
//   1 ÓĞÓ¦´ğ
//   2 ÎŞÓ¦´ğ
void I2C_SendByte(u8 SendByte) //Êı¾İ´Ó¸ßµ½µÍ  ·¢ËÍÊı¾İÊÇSDA¸ßµÄÊ±¼äÑÛ´óÓÚSCL¸ßµÄÊ±¼ä 
{
    u8 i=8;
    while(i--)  //8Î»=1×Ö½ÚÊı¾İ ·¢ËÍ8´Î
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80) //·¢ËÍµÄÊı¾İ×î¸ßÎ»ÊÇ1
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;  //×óÒÆ£¬´Î¸ßÎ»±ä¸ßÎ»
        I2C_delay();
		SCL_H;             
        I2C_delay();
    }
    SCL_L;
}  



//¶ÁÈ¡Ò»¸ö×Ö½ÚÊı¾İ
unsigned char I2C_RadeByte(void)  //Êı¾İ´Ó¸ßÎ»µ½µÍÎ»£¬ ¶ÁÊı¾İSCL¸ßµçÆ½Ê±¼äÒª´óÓÚSDAÊ±¼ä
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;   //µÃµ½Ò»¸öÊı×óÒÆÒ»Î»   
      SCL_L;
      I2C_delay();
	    SCL_H;
      I2C_delay();	
      if(SDA_read)      //SDA¶Áµ½¸ßµçÆ½
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 


//iicµ¥×Ö½ÚĞ´Èëº¯Êı(Æô¶¯Íâ½ÓĞ¾Æ¬ÓÃ) slave addressÆ÷¼şµØÖ·  REG_Address¼Ä´æÆ÷µØÖ·£    REG_data¼Ä´æÆ÷Ğ´ÈëÖµ
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //·¢ËÍÉè±¸µØÖ·+Ğ´ĞÅºÅ(0)
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //·¢ËÍ¼Ä´æÆ÷µØÖ·     
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}



//iicµ¥×Ö½Ú¶ÁÈ¡º¯Êı(¶ÁÍâ½ÓĞ¾Æ¬ÓÃ)   slave addressÍâ²¿Ğ¾Æ¬µØÖ·   REG_Address´ÓÉè±¸µÄ¼Ä´æÆ÷µØÖ·
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data; //´æ·Å¶¼³öÀ´µÄÊı¾İ    	
	if(!I2C_Start())return FALSE; //¼ì²éI2CÊÇ·ñ¿ªÊ¼
    I2C_SendByte(SlaveAddress); //·¢ËÍÉè±¸µØÖ·+Ğ´ĞÅºÅ(0)
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;} //¼ì²éI2CÊÇ·ñÓĞÓ¦´ğ
    I2C_SendByte((u8) REG_Address);   //·¢ËÍ¼Ä´æÆ÷µØÖ·   
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1); //·¢ËÍÉè±¸µØÖ·+¶ÁĞÅºÅ(1)
    I2C_WaitAck();

	  REG_data= I2C_RadeByte(); 
    I2C_NoAck();  //²»²úÉúÓ¦´ğĞÅºÅ£¬°Ñ¶ÁÊı¾İÀ­Æğ¡£ÈÃËûµÈ´ı²»ÈÃËû¼ÌĞø´«´ïÊı¾İ
    I2C_Stop();                                  //·ñÔòÓÖÓ¦´ğĞÅºÅ½ÓÊÕ³É¹¦ ÓÖ¼ÌĞø½ÓÊÕÊı¾İÁË
    //return TRUE;
	return REG_data; //·µ»Ø¶Áµ½µÄÊı¾İ

}						      
	


//ÊıÖµ×ª»»º¯Êı 
void DATA_printf(uchar *s,short temp_data)  //°Ñtemp_dataµÄÖµ °´Î» ¸³¸øs£¬ÒòÎª²âÁ¿µÄÖµ¿ÉÄÜÎª¸º
{
	if(temp_data<0)   //Èç¹û²âÁ¿µÄÖµÎª¸ºÊı£¬¾ÍÏÈ¸ø s ¸³ÖµÒ»¸ö¸ººÅ
		{
	   temp_data=-temp_data;
     *s='-';
	  }
	else *s='+';     //·ñÔò¸³ÖµÕıºÅ
    *++s =temp_data/100+0x30;     //È¡°ÙÎ»        ascÂë0x30ÊÇ0 temp_data%100¾Í°Ñtemp_data±äÎªÖ»º¬Ê®Î»ÒÔºó
    temp_data=temp_data%100;     
    *++s =temp_data/10+0x30;      //È¡Ê®Î»
    temp_data=temp_data%10;      
    *++s =temp_data+0x30; 	      //È¡¸öÎ»
}
//Õı¸ººÅ¼ÓÈıÎ»Êı¾İ·¢¸øÖ¸Õës  sÖ¸ÏòÊı×éTX_DATA  ÔÙ°ÑÊı×éTX_DATAµÄËÄ¸öÊı¾İ°´Î»·¢ËÍ

