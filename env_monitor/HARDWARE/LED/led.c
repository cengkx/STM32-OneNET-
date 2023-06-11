#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = LED1_Pin;				 //PA1�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(LED_Port, &GPIO_InitStructure);				 //��ʼ��IO��
 	GPIO_SetBits(LED_Port,LED1_Pin);						 //PA1 �����
			    
}

