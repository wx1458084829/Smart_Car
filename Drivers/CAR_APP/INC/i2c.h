#ifndef __I2C_H
#define __I2C_H

#include "sys.h"


#define MPU6050_PORT GPIOF


#define  MPU6050_I2C_SDA_1() HAL_GPIO_WritePin(MPU6050_PORT, MPU6050_SDA_Pin, GPIO_PIN_SET)
#define  MPU6050_I2C_SCL_1() HAL_GPIO_WritePin(MPU6050_PORT, MPU6050_SCL_Pin, GPIO_PIN_SET)

#define  MPU6050_I2C_SDA_0() HAL_GPIO_WritePin(MPU6050_PORT, MPU6050_SDA_Pin, GPIO_PIN_RESET)
#define  MPU6050_I2C_SCL_0() HAL_GPIO_WritePin(MPU6050_PORT, MPU6050_SCL_Pin, GPIO_PIN_RESET)

#define MPU6050_I2C_SDA_READ() HAL_GPIO_ReadPin(MPU6050_PORT, MPU6050_SDA_Pin)


void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t _ucByte);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_Read_Byte(uint8_t ack);







#endif  /** __I2C_H */

