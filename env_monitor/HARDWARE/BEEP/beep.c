#include "beep.h"
#include "stm32f10x.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //��������ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = BEEP_Pin;				 //PA15�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(BEEP_Port, &GPIO_InitStructure);				 //��ʼ��IO��
 	GPIO_ResetBits(BEEP_Port,BEEP_Pin);						 //PA15 �����
			
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //Ĭ����Ϊ���Խӿ����š�����JTAG
}

