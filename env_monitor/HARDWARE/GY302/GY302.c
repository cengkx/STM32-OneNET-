#include "GY302.h"

uint8_t dataBUF[3];

void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void SDA_Output( uint8_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SDA_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SDA_Pin);
	}
}


void SCL_Output( uint8_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SCL_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SCL_Pin);
	}
}

uint8_t SDA_Input()
{
	return GPIO_ReadInputDataBit( I2C_PORT, SDA_Pin);
}

/***��ʼ�ź�**/
void GY302_Start()
{
    SDA_Output(1);                   //����������
    SCL_Output(1);                   //����ʱ����
    delay_us(5);
    SDA_Output(0);                   //�����½���
    delay_us(5);
    SCL_Output(0);                   //����ʱ����
}

/*****ֹͣ�ź�******/
void GY302_Stop()
{
    SDA_Output(0);                   //����������
    SCL_Output(1);                   //����ʱ����
    delay_us(5);
    SDA_Output(1);                   //����������
    delay_us(5);
}
/*****��ʼ��GY302******/
void GY302_Init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//��������GY302�ڲ�����
void mread(void)
{
    uint8_t i;
    GY302_Start();                          //��ʼ�ź�
    GY302_SendByte(SlaveAddress+1);         //�����豸��ַ��+���ź�

     for (i=0; i<3; i++)                      //������ȡ6����ַ���ݵ�dataBUF
    {
        dataBUF[i] = GY302_RecvByte();
        if (i == 3)
        {
           GY302_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          GY302_SendACK(0);                //��ӦACK
        }
   }

    GY302_Stop();                          //ֹͣ�ź�
    delay_us(5);

}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void GY302_SendACK(int ack)
{
    SDA_Output_Mode();

    if(ack == 1)   //дӦ���ź�
        SDA_Output(1);
    else if(ack == 0)
        SDA_Output(0);
    else
        return;

    SCL_Output(1);
    delay_us(5);
    SCL_Output(0);
    delay_us(5);
}

/**************************************
����Ӧ���ź�
**************************************/
uint8_t GY302_RecvACK()
{
	uint8_t mcy = 0;

    SDA_Input_Mode();

    SCL_Output(1);              //����ʱ����
    delay_us(5);

    if(SDA_Input() == 1 )//��Ӧ���ź�
        mcy = 1 ;
    else
        mcy = 0 ;

    SCL_Output(0);                    //����ʱ����
    delay_us(5);

    SDA_Output_Mode();

    return mcy;
}

/**************************************
��iic���߷���һ���ֽ�����
**************************************/
void GY302_SendByte(uint8_t dat)
{
    uint8_t i;

    for (i=0; i<8; i++)         //8λ������
      {
        if( 0X80 & dat )
            SDA_Output(1);
        else
            SDA_Output(0);

        dat <<= 1;
        SCL_Output(1);               //����ʱ����
        delay_us(5);
        SCL_Output(0);                //����ʱ����
        delay_us(5);
      }
    GY302_RecvACK();
}

//���Ƕ�GY302��������ʱ��Ҫ�ȷ���������ַ+д��λ��Ȼ����ָ��
//��ȡ���ݵ�ʱ����Ҫ�ȷ���������ַ+����λ��Ȼ���ȡ���ֽ�����

//д��ָ��
void Single_Write_GY302(uint8_t REG_Address)//REG_Address��Ҫд���ָ��
{
    GY302_Start();                  //��ʼ�ź�
    GY302_SendByte(SlaveAddress);  //����������ַ+д�ź�
    GY302_SendByte(REG_Address);   //д��ָ��ڲ��Ĵ�����ַ
    GY302_Stop();                   //�����ź�
}
/**************************************
��iic���߶�ȡһ���ֽ�����
**************************************/
uint8_t GY302_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    uint8_t bit;

    SDA_Input_Mode();

    SDA_Output(1);          //׼����ȡ����
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL_Output(1);               //����ʱ����
        delay_us(5);

        if( SET == SDA_Input() )
            bit = 0X01;
        else
            bit = 0x00;

        dat |= bit;             //������

        SCL_Output(0);                //����ʱ����
        delay_us(5);
    }

    SDA_Output_Mode();

    return dat;
}

uint32_t readGY302(void)
{
    uint16_t dis_data;
    uint16_t Value_GY_302;

    Single_Write_GY302(GY302_PWR_ON);   // power on
    Single_Write_GY302(GY302_CON_H);   // H- resolution mode
    delay_ms(180);            //��ʱ180ms
    mread();       //�����������ݣ��洢��dataBUF��
    dis_data = dataBUF[0];
    dis_data = (dis_data<<8)+dataBUF[1];//�ֽںϳ�����
    Value_GY_302 = (uint32_t)(dis_data/1.2);
    return Value_GY_302;
}
