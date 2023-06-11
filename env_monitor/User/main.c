//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//Ӳ������
#include "oled.h"
#include "delay.h"
#include "dht11.h"
#include "MQ-2.h"
#include "usart.h"
#include "GY302.h"
#include "bt.h"
#include "beep.h"
#include "led.h"

#include "sys.h"

#include <string.h>

u8 temperature_h;//����
u8 temperature_l;//С��	
u8 humidity_h;
u8 humidity_l;
uint16_t mq2data;
uint32_t gy302data;

//��������ֵ��ʼֵ
u8 warn_temperature=40;
u8 warn_humidity=60;
u16 warn_smog=80;
u32 warn_light=500;

void enviroment_check()
{
	if(temperature_h < warn_temperature && humidity_h < warn_humidity && mq2data<warn_smog && gy302data<warn_light)
	{
		LED1_OFF();BEEP_OFF();
	}
	else
	{
		LED1_ON();BEEP_ON(); 
	}
}

//void uart_revpro(void)
//{
//	u16 len;
//	u16 t;
//	
//	short result = 0;
//	char *dataPtr = NULL;
//	char numBuf[10];
//	int num = 0;
//	
//	if(USART_RX_STA&0x3fff)
//		{			   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			
//			UsartPrintf(USART1, "\r\n�����͵���ϢΪ:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			UsartPrintf(USART1, "\r\n\r\n");//���뻻��
//			
//		dataPtr = strchr((const char*)USART_RX_BUF, ':');					//����':'

//		if(dataPtr != NULL && result != -1)					//����ҵ���
//		{
//			dataPtr++;
//			
//			while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
//			{
//				numBuf[num++] = *dataPtr++;
//			}
//			numBuf[num] = 0;
//			
//			num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
//			
//			if(strstr((char *)USART_RX_BUF, "warn_temp"))		//����"warn_temp"
//			{
//				warn_temperature=num;
//			}
//			else if(strstr((char *)USART_RX_BUF, "warn_humi"))		//����"warn_humi"
//			{
//				warn_humidity=num;
//			}
//			else if(strstr((char *)USART_RX_BUF, "warn_smog"))		//����"warn_smog"
//			{
//				warn_smog=num;
//			}
//			else if(strstr((char *)USART_RX_BUF, "warn_light"))		//����"warn_humi"
//			{
//				warn_light=num;
//			}
//				
//				USART_RX_STA=0;
//		}
//	}
//}

void uart_revpro(void)
{
	u16 len;
	u16 t;
	
	short result = 0;
	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	USART_TypeDef* USART_REC;
	u8* USART_RX_BUF_REC;
	u16* USART_RX_STA_REC;
	
	if(USART_RX_STA&0x8000||USART2_RX_STA&0x8000)  
		{			   
			if(USART_RX_STA&0x3fff) //�յ�������ǵ��Դ���
			{
				len=USART_RX_STA&0x8000;//�õ��˴ν��յ������ݳ���
				USART_REC=USART_DEBUG;
				USART_RX_BUF_REC=USART_RX_BUF;
				USART_RX_STA_REC=&USART_RX_STA;
			}
			else //�յ�������������
			{
				len=USART2_RX_STA&0x8000;//�õ��˴ν��յ������ݳ���
				USART_REC=USART_BT;
				USART_RX_BUF_REC=USART2_RX_BUF;
				USART_RX_STA_REC=&USART2_RX_STA;
			}
			UsartPrintf(USART_REC, "\r\n�յ���ָ��Ϊ:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART_REC, USART_RX_BUF_REC[t]);//����մ��ڷ�������
				while(USART_GetFlagStatus(USART_REC,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			UsartPrintf(USART_REC, "\r\n\r\n");//���뻻��
			
		dataPtr = strchr((const char*)USART_RX_BUF_REC, ':');					//����':'

		if(dataPtr != NULL && result != -1)					//����ҵ���
		{
			dataPtr++;
			while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
			{
				numBuf[num++] = *dataPtr++;
			}
			numBuf[num] = 0;
			
			num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
			
			if(strstr((char *)USART_RX_BUF_REC, "warn_temp"))		//����"warn_temp"
				warn_temperature=num;
			else if(strstr((char *)USART_RX_BUF_REC, "warn_humi"))		//����"warn_humi"
				warn_humidity=num;
			else if(strstr((char *)USART_RX_BUF_REC, "warn_smog"))		//����"warn_smog"
				warn_smog=num;
			else if(strstr((char *)USART_RX_BUF_REC, "warn_light"))		//����"warn_humi"
				warn_light=num;	
		}
		*USART_RX_STA_REC=0;
	}
}

 int main(void)
{
//	u8 t;
  u16 timeCount = 0;	//���ͼ������
	u8  *dataPtr = NULL;
	
	char tempStr[20] = {0}, humiStr[20] = {0};
//	char mq2Str[20] = {0};
	char gyStr[20] = {0};
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	OLED_Init();			//��ʼ��OLED  
	MQ2_Init();
	LED_Init();
	BEEP_Init();
	GY302_Init();
	uart1_init(115200);//����1������ͨ��
	uart2_init(9600);//����2����������
	uart3_init(115200);//����3������ESP8266
//	while(DHT11_Init())	//DHT11��ʼ��	
//	{
//		OLED_ShowString(0,0,"DHT11 checking");
//		delay_ms(800);
//		OLED_Clear();
// 		delay_ms(200);
//	}	
	DHT11_Init();
	 
	OLED_Clear();
	
	OLED_ShowString(0,0,"ESP8266 ");
	ESP8266_Init();					//��ʼ��ESP8266
	OLED_Clear();
	
	OLED_ShowString(0,0,"ONENET ");
	BEEP_ON();
	LED1_ON();
	delay_ms(800);
	BEEP_OFF();
	LED1_OFF();
	
 	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	OLED_Clear();
	
//	t=' ';
  while(1)
	{
//		printf("ceshi!\r\n");
//		UsartPrintf(USART_DEBUG, "uart1:%d\r\n",USART_RX_STA&0x8000);
//		UsartPrintf(USART_BT, "uart2:%d\r\n",USART2_RX_STA&0x8000);
	  OLED_Clear();
		DHT11_Read_Data(&temperature_h,&temperature_l,&humidity_h,&humidity_l);	//��ȡ��ʪ��ֵ	
		sprintf(tempStr, "Temp:%d.%dC", temperature_h, temperature_l);
		OLED_ShowString(0,0,tempStr);
//		printf("Temp:%d.%dC", temperature_h, temperature_l);
		
		sprintf(humiStr, "Humi: %d%%", humidity_h);
		OLED_ShowString(0,2,humiStr);
		
//ԭ��
//		DHT11_Read_Data(&temperature_h,&humidity_h);	//��ȡ��ʪ��ֵ					    
//		OLED_ShowString(0,0,"Temp:");
//		OLED_ShowNum(40,0,temperature_h,2,16);	
//		
//		OLED_ShowString(0,2,"Humi:");
//		OLED_ShowNum(40,2,humidity_h,2,16);	

		
		
		mq2data=readMQ2();
		OLED_ShowString(0,4,"Smog:");
		OLED_ShowNum(40,4,mq2data,2,16);
		
		gy302data=readGY302();
		sprintf(gyStr, "Light: %d", gy302data);
		OLED_ShowString(0, 6, gyStr);
		
		
		
//		USART2_SendString("test");
//		USART_SendData(USART2, 'k');
//		USART_SendData(USART1, 'k');
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);


		
//		OLED_ShowCHinese(0,0,0);//��
//		OLED_ShowCHinese(16,0,1);//��
//		OLED_ShowCHinese(32,0,2);//��
//		OLED_ShowCHinese(48,0,3);//��
//		OLED_ShowString(0,3,"1.3' OLED TEST");
//		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
//	 //	OLED_ShowString(20,4,"2014/05/01");  
//		OLED_ShowString(0,6,"ASCII:");  
//		OLED_ShowString(63,6,"CODE:");  
//		OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			
		if(++timeCount >= 10)									//���ͼ��
		{
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//��������
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		enviroment_check();
		uart_revpro();
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		
		
		delay_ms(10000);

	}
}

