#ifndef __BT_H
#define __BT_H
#include "stdio.h"	
#include "sys.h" 

#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	

#define USART_BT		USART2		//���Դ�ӡ��ʹ�õĴ����� 

void uart2_init(u32 bound);
void USART2_SendString(u8* Str);


#endif
