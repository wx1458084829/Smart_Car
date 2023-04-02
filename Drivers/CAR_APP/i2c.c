#include "i2c.h"

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}


static void sda_Read_Mode(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = MPU6050_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MPU6050_PORT, &GPIO_InitStruct);
}


static void sda_Write_Mode(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = MPU6050_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(MPU6050_PORT, &GPIO_InitStruct);
}



/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	MPU6050_I2C_SDA_1();
	MPU6050_I2C_SCL_1();
	i2c_Delay();
	MPU6050_I2C_SDA_0();
	i2c_Delay();
	MPU6050_I2C_SCL_0();
	i2c_Delay();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	MPU6050_I2C_SDA_0();
	MPU6050_I2C_SCL_1();
	i2c_Delay();
	MPU6050_I2C_SDA_1();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Send_Byte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			MPU6050_I2C_SDA_1();
		}
		else
		{
			MPU6050_I2C_SDA_0();
		}
		i2c_Delay();
		MPU6050_I2C_SCL_1();
		i2c_Delay();	
		MPU6050_I2C_SCL_0();
		if (i == 7)
		{
			 MPU6050_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
	
		sda_Read_Mode();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t re;
	MPU6050_I2C_SDA_1();	/* CPU释放SDA总线 */

	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (MPU6050_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}	
	MPU6050_I2C_SCL_0();
	i2c_Delay();
  sda_Write_Mode();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Ack(void)
{
	MPU6050_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	MPU6050_I2C_SCL_0();
	i2c_Delay();
	MPU6050_I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_NAck(void)
{
	
	
	MPU6050_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	MPU6050_I2C_SCL_0();
	i2c_Delay();	
}


/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i;
	uint8_t value;
	sda_Read_Mode();
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		MPU6050_I2C_SCL_1();
		i2c_Delay();
		if (MPU6050_I2C_SDA_READ())
		{
			value++;
		}
		MPU6050_I2C_SCL_0();
		i2c_Delay();
	}
	sda_Write_Mode();
	
	if(ack ==0)
	{
		IIC_NAck();
	}else{
		IIC_Ack();
	}
	
	return value;
}

