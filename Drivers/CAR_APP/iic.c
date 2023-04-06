#include "iic.h"
static uint8_t fac_us = 72;    //������ʱ��Ϊ72M, ������1us��ticks���72��

/*
  * @brief	��ʱ����
  * @note   ��ʱ΢�뺯��
  * @param  ΢��
  * @retval void
  */
static void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//װ��ֵ
	ticks=nus*fac_us; //��Ҫ�Ľ�����
	told=SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//�������ݼ�
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;	//ʱ�䳬��������ӳٵ�ʱ��ʱ�˳�.
		}
	};
}

void IIC_Gpio_Clk_Cmd(uint32_t RCC_APB2ENR_IOPxEN)
{
	 do { \
       __IO uint32_t tmpreg; \
       SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPxEN);\
       /* Delay after an RCC peripheral clock enabling */\
       tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPxEN);\
       UNUSED(tmpreg); \
   } while(0U);
}

void IIC_Init(IIC_TypeDef *iic)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	//�жϿ���ʱ��
	if(iic->SCL_Enable_Clk){
		IIC_Gpio_Clk_Cmd(iic->SCL_Enable_Clk);
	}
	if(iic->SDA_Enable_Clk){
		IIC_Gpio_Clk_Cmd(iic->SDA_Enable_Clk);
	}
	
  // ����SDA����
  GPIO_InitStruct.Pin = iic->SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // ��ʼ��SDA
  HAL_GPIO_Init(iic->SDA_Port, &GPIO_InitStruct);

  // ����SCL����
  GPIO_InitStruct.Pin = iic->SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // ��ʼ��SCL
  HAL_GPIO_Init(iic->SCL_Port, &GPIO_InitStruct);

  //����ֹͣ�ź�
  IIC_Stop(iic);
}

/**
 * ����ֹͣ�ź�
 */
void IIC_Start(IIC_TypeDef *iic)
{
  // ׼��������ʼ�ź�
  IIC_SDA_1(iic);
  IIC_SCL_1(iic);
  delay_us(1);
  //������ʼ�ź�
  IIC_SDA_0(iic);
  delay_us(1);
}

/**
 * ֹͣ�ź�
 */
void IIC_Stop(IIC_TypeDef *iic)
{
  //׼������ֹͣ�ź�
  IIC_SDA_0(iic);
  IIC_SCL_1(iic);
  delay_us(1);
  // ����ֹͣ�ź�
  IIC_SDA_1(iic);
}

/**
 * ��������
 */
void IIC_SendByte(IIC_TypeDef *iic,uint8_t _ucByte)
{
	uint8_t i;
	IIC_SCL_0(iic);
 /* д7bit���� */
	for (i = 0; i < 8; i++) {
		if (_ucByte & 0x80) {
			IIC_SDA_1(iic);
		}else{
			IIC_SDA_0(iic);
		}
		  delay_us(1);
		IIC_SCL_1(iic);
		  delay_us(1);
		IIC_SCL_0(iic);
		_ucByte <<= 1; /* ����һ��bit */
		delay_us(1);
	}
	IIC_SDA_1(iic);
	delay_us(1);
}

/**
 * ��ȡ����
 */
uint8_t IIC_ReadByte(IIC_TypeDef *iic,uint8_t isAck)
{
	uint8_t i;
	uint8_t value;

	/* ��ȡ���� */
	value = 0;
	for (i = 0; i < 8; i++) {
		value <<= 1;
		IIC_SCL_1(iic);
		delay_us(1);
		if (IIC_SDA_READ(iic)) {
			value++;
		}
		IIC_SCL_0(iic);
		delay_us(1);
	}
	if(isAck){
	IIC_Ack(iic);
	}else{
	IIC_NAck(iic);
	}
	
	
	return value;
}

/**
 * �ȴ����豸��Ӧ
 */
uint8_t IIC_WaitAck(IIC_TypeDef *iic)
{
	uint8_t re;
	IIC_SDA_1(iic);  //�ͷ�SDA
	delay_us(1);
	IIC_SCL_1(iic); //����SCL����
	delay_us(1);
	if (IIC_SDA_READ(iic))  //��ȡSDA�ߣ��ж�slave��û�з���ACK
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_0(iic); //����SCL���ߣ���ȡȷ���ź����
	delay_us(1);
	return re;
}

/**
 * ����ACK
 */
void IIC_Ack(IIC_TypeDef *iic)
{
	IIC_SDA_0(iic);	//����SDA�ߣ���ACK�ź�
	delay_us(1);
	IIC_SCL_1(iic);	//����SCL
	delay_us(1);
	IIC_SCL_0(iic);	//����SCL������ACK���
	delay_us(1);
	IIC_SDA_1(iic);	//�ͷ�SDA
}
/**
 * ����NCK
 */
void IIC_NAck(IIC_TypeDef *iic)
{
	IIC_SDA_1(iic);	//����SDA�ߣ���NACK�ź�
	delay_us(1);
	IIC_SCL_1(iic);	//����SCL��
	delay_us(1);
	IIC_SCL_0(iic);	//����SCL�ߣ�����NACK���
	delay_us(1);
}
