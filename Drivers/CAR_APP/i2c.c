#include "i2c.h"

static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	MPU6050_I2C_SDA_0();
	MPU6050_I2C_SCL_1();
	i2c_Delay();
	MPU6050_I2C_SDA_1();
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Send_Byte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 MPU6050_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
	
		sda_Read_Mode();
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t re;
	MPU6050_I2C_SDA_1();	/* CPU�ͷ�SDA���� */

	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (MPU6050_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Ack(void)
{
	MPU6050_I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	MPU6050_I2C_SCL_0();
	i2c_Delay();
	MPU6050_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_NAck(void)
{
	
	
	MPU6050_I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	MPU6050_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	MPU6050_I2C_SCL_0();
	i2c_Delay();	
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i;
	uint8_t value;
	sda_Read_Mode();
	/* ������1��bitΪ���ݵ�bit7 */
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

