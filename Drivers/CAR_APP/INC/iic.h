#ifndef __IIC_H
#define __IIC_H
#include "main.h"


//!!!!必须在cubemax里面开启时钟，或者单独开启时钟，之后可以用寄存器实现
//	uint32_t RCC_APB2ENR_IOPxEN; x为IO端口
//IIC结构体定义
typedef struct{
	uint32_t SDA_Pin;
	uint32_t SCL_Pin;
	GPIO_TypeDef *SDA_Port;
	GPIO_TypeDef *SCL_Port;
	uint32_t SDA_Enable_Clk;
	uint32_t SCL_Enable_Clk;
} IIC_TypeDef;


// 端口电平读写宏定义
#define IIC_SDA_1(iic) HAL_GPIO_WritePin(iic->SDA_Port, iic->SDA_Pin, GPIO_PIN_SET)
#define IIC_SDA_0(iic) HAL_GPIO_WritePin(iic->SDA_Port, iic->SDA_Pin, GPIO_PIN_RESET)

#define IIC_SCL_1(iic) HAL_GPIO_WritePin(iic->SCL_Port, iic->SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_0(iic) HAL_GPIO_WritePin(iic->SCL_Port, iic->SCL_Pin, GPIO_PIN_RESET)



// 读数据宏定义
#define IIC_SDA_READ(iic) HAL_GPIO_ReadPin(iic->SDA_Port, iic->SDA_Pin)
#define IIC_SCL_READ(iic) HAL_GPIO_ReadPin(iic->SCL_Port, iic->SCL_Pin)

//IIC函数
void IIC_Init(IIC_TypeDef *iic);
void IIC_Start(IIC_TypeDef *iic);
void IIC_Stop(IIC_TypeDef *iic);
void IIC_SendByte(IIC_TypeDef *iic,uint8_t _ucByte);
uint8_t IIC_ReadByte(IIC_TypeDef *iic,uint8_t isAck);
uint8_t IIC_WaitAck(IIC_TypeDef *iic);
void IIC_Ack(IIC_TypeDef *iic);
void IIC_NAck(IIC_TypeDef *iic);

#endif  /** __IIC_H */

