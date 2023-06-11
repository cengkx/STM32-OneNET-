#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DHT11������ʪ�ȴ�������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//IO��������
#define DHT11_IO_IN()  {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=8;}
#define DHT11_IO_OUT() {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=3;}
////IO��������											   
#define	DHT11_DQ_OUT PBout(0) //���ݶ˿�	PB0
#define	DHT11_DQ_IN  PBin(0)  //���ݶ˿�	PB0


u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp_high,u8 *temp_low,u8 *humi_high,u8 *humi_low);//��ȡ��ʪ��
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif

////IO��������
//#define DHT11_IO_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
//#define DHT11_IO_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
//////IO��������											   
//#define	DHT11_DQ_OUT PBout(11) //���ݶ˿�	PB11
//#define	DHT11_DQ_IN  PBin(11)  //���ݶ˿�	PB11


//u8 DHT11_Init(void);//��ʼ��DHT11
//u8 DHT11_Read_Data(u8 *temp_high,u8 *temp_low,u8 *humi_high,u8 *humi_low);//��ȡ��ʪ��
////u8 DHT11_Read_Data(u8 *temp,u8 *humi);
//u8 DHT11_Read_Byte(void);//����һ���ֽ�
//u8 DHT11_Read_Bit(void);//����һ��λ
//u8 DHT11_Check(void);//����Ƿ����DHT11
//void DHT11_Rst(void);//��λDHT11    
//#endif













