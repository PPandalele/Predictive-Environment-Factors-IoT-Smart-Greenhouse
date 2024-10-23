#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
//#include "lcd.h"
#include "dht11.h"
#include "myiic.h"
#include "bh1750.h" 
#include "soil.h"
#include "SGP30.h"
#include "beep.h"
#include "bmpiic.h"
#include "bmp280.h" 
//STM32F407¿ª·¢°å ÊµÑé31
//DHT11Êý×ÖÎÂÊª¶È´«¸ÐÆ÷ ÊµÑé  
//ÌÔ±¦µêÆÌ£ºhttp://CHINANUEDC.taobao.com  
u32 CO2Data,TVOCData;//¶¨ÒåCO2Å¨¶È±äÁ¿ÓëTVOCÅ¨¶È±

int b=26;
 
int main(void)
{ 
	u32 sgp30_dat;
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity; 
//	u16 temp; 
	u8 light; 
	double	BMP_Pressure,BMP_Temperature;
//	float soilhumidity;	

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);  //³õÊ¼»¯ÑÓÊ±º¯Êý
	uart_init(115200);		//³õÊ¼»¯´®¿Ú²¨ÌØÂÊÎª115200
	delay_ms(100);//µÈ´ýÄ£¿é³õÊ¼»¯Íê³É
	

	BEEP_Init();
	LED_Init();					//³õÊ¼»¯LED 
	Adc_Init();	
	IIC_Init();
	BMP_IIC_Init();
	SGP30_Init();   //³õÊ¼»¯SGP30
	delay_ms(100);
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//¶ÁÈ¡SGP30µÄÖµ
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	TVOCData = sgp30_dat & 0x0000ffff;

 	while(DHT11_Init())	//DHT11³õÊ¼»¯	
	{
		printf("\r\nDHT11 Error\r\n");

	}								   

	printf("\r\nDHT11 OK\r\n");

	
		while(CO2Data == 400 && TVOCData == 0)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//¶ÁÈ¡SGP30µÄÖµ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//È¡³öCO2Å¨¶ÈÖµ
		TVOCData = sgp30_dat & 0x0000ffff;			 //È¡³öTVOCÖµ
		printf("ÕýÔÚ¼ì²âÖÐ...\r\n");
		delay_ms(500);
	}
	
	Bmp_Init();
	delay_ms(50);
	/*Configuration Operation*/
	
	printf("\r\nBMP280 ID:0x%d",BMP280_ReadID());	
	delay_ms(50);
	while(1)
	{	    	    
 		if(t%10==0)//Ã¿100ms¶ÁÈ¡Ò»´Î
		{									  
			DHT11_Read_Data(&temperature,&humidity);		//¶ÁÈ¡ÎÂÊª¶ÈÖµ		
			SGP30_Write(0x20,0x08);
		  sgp30_dat = SGP30_Read();//¶ÁÈ¡SGP30µÄÖµ
			//light = Read_BH1750_Data();
		  /*CO2Data = (sgp30_dat & 0xffff0000) >> 16;//È¡³öCO2Å¨¶ÈÖµ
		  TVOCData = sgp30_dat & 0x0000ffff;       //È¡³öTVOCÖµ
		  printf("CO2:%dppm\r\nTVOC:%dppd\r\n",CO2Data,TVOCData);
		  delay_ms(500);*/
//			light = Read_BH1750_Data();			
//			
//			temp=Get_Adc_Average(ADC_Channel_5,5);//»ñÈ¡Í¨µÀ5µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù

//			soilhumidity=(float)temp*(3.3/1024);          //»ñÈ¡¼ÆËãºóµÄ´øÐ¡ÊýµÄÊµ¼ÊµçÑ¹Öµ£¬±ÈÈç3.1111

 	   
		}				   
	 	delay_ms(10);
		t++;
		if(t==200)
		{
			t=0;
			LED0=!LED0;
			light = Read_BH1750_Data();
			CO2Data = (sgp30_dat & 0xffff0000) >> 16;//È¡³öCO2Å¨¶ÈÖµ
		  TVOCData = sgp30_dat & 0x0000ffff;       //È¡³öTVOCÖµ

			if(temperature>b)
		  {  Sound2(100);  }
			//printf("temperature=%d humidity=%d light=%d soilhumidity=%0.1f \r\n",temperature,humidity,light,soilhumidity);
			printf("temperature=%d humidity=%d light=%d\r\n",temperature,humidity,light);
			printf("CO2:%dppm\r\nTVOC:%dppd\r\n",CO2Data,TVOCData);
			
			while(BMP280_GetStatus(BMP280_MEASURING) != RESET);
			while(BMP280_GetStatus(BMP280_IM_UPDATE) != RESET);
			BMP_Temperature = BMP280_Get_Temperature();
			BMP_Pressure = BMP280_Get_Pressure();
			printf("\r\nTemperature %f C Pressure %f Pa",BMP_Temperature,BMP_Pressure);
		}
	
	}
}






