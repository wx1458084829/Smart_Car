#include "iic.h"
static uint8_t fac_us = 72;    //这里主时钟为72M, 所以在1us内ticks会减72次

/*
  * @brief	延时函数
  * @note   延时微秒函数
  * @param  微秒
  * @retval void
  */
static void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//装载值
	ticks=nus*fac_us; //需要的节拍数
	told=SysTick->VAL; //刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//计数器递减
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;	//时间超过或等于延迟的时间时退出.
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
	
	//判断开启时钟
	if(iic->SCL_Enable_Clk){
		IIC_Gpio_Clk_Cmd(iic->SCL_Enable_Clk);
	}
	if(iic->SDA_Enable_Clk){
		IIC_Gpio_Clk_Cmd(iic->SDA_Enable_Clk);
	}
	
  // 设置SDA参数
  GPIO_InitStruct.Pin = iic->SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // 初始化SDA
  HAL_GPIO_Init(iic->SDA_Port, &GPIO_InitStruct);

  // 设置SCL参数
  GPIO_InitStruct.Pin = iic->SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // 初始化SCL
  HAL_GPIO_Init(iic->SCL_Port, &GPIO_InitStruct);

  //发送停止信号
  IIC_Stop(iic);
}

/**
 * 发送停止信号
 */
void IIC_Start(IIC_TypeDef *iic)
{
  // 准备发送起始信号
  IIC_SDA_1(iic);
  IIC_SCL_1(iic);
  delay_us(1);
  //发送起始信号
  IIC_SDA_0(iic);
  delay_us(1);
}

/**
 * 停止信号
 */
void IIC_Stop(IIC_TypeDef *iic)
{
  //准备发送停止信号
  IIC_SDA_0(iic);
  IIC_SCL_1(iic);
  delay_us(1);
  // 发送停止信号
  IIC_SDA_1(iic);
}

/**
 * 发送数据
 */
void IIC_SendByte(IIC_TypeDef *iic,uint8_t _ucByte)
{
	uint8_t i;
	IIC_SCL_0(iic);
 /* 写7bit数据 */
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
		_ucByte <<= 1; /* 左移一个bit */
		delay_us(1);
	}
	IIC_SDA_1(iic);
	delay_us(1);
}

/**
 * 读取数据
 */
uint8_t IIC_ReadByte(IIC_TypeDef *iic,uint8_t isAck)
{
	uint8_t i;
	uint8_t value;

	/* 读取数据 */
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
 * 等待从设备响应
 */
uint8_t IIC_WaitAck(IIC_TypeDef *iic)
{
	uint8_t re;
	IIC_SDA_1(iic);  //释放SDA
	delay_us(1);
	IIC_SCL_1(iic); //拉升SCL总线
	delay_us(1);
	if (IIC_SDA_READ(iic))  //读取SDA线，判断slave有没有发送ACK
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_0(iic); //拉低SCL总线，读取确认信号完成
	delay_us(1);
	return re;
}

/**
 * 发送ACK
 */
void IIC_Ack(IIC_TypeDef *iic)
{
	IIC_SDA_0(iic);	//拉低SDA线，是ACK信号
	delay_us(1);
	IIC_SCL_1(iic);	//拉升SCL
	delay_us(1);
	IIC_SCL_0(iic);	//拉低SCL，发送ACK完成
	delay_us(1);
	IIC_SDA_1(iic);	//释放SDA
}
/**
 * 发送NCK
 */
void IIC_NAck(IIC_TypeDef *iic)
{
	IIC_SDA_1(iic);	//拉高SDA线，是NACK信号
	delay_us(1);
	IIC_SCL_1(iic);	//拉升SCL线
	delay_us(1);
	IIC_SCL_0(iic);	//拉低SCL线，发送NACK完成
	delay_us(1);
}
