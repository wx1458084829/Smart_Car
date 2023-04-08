/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_A2_Pin GPIO_PIN_3
#define MOTOR_A2_GPIO_Port GPIOC
#define MOTOR_A1_Pin GPIO_PIN_3
#define MOTOR_A1_GPIO_Port GPIOA
#define OLED_SCL_Pin GPIO_PIN_4
#define OLED_SCL_GPIO_Port GPIOA
#define OLED_SDA_Pin GPIO_PIN_5
#define OLED_SDA_GPIO_Port GPIOA
#define MPU6050_SCL_Pin GPIO_PIN_12
#define MPU6050_SCL_GPIO_Port GPIOF
#define MPU6050_SDA_Pin GPIO_PIN_13
#define MPU6050_SDA_GPIO_Port GPIOF
#define ENCODER_A1_Pin GPIO_PIN_9
#define ENCODER_A1_GPIO_Port GPIOE
#define ENCODER_A2_Pin GPIO_PIN_11
#define ENCODER_A2_GPIO_Port GPIOE
#define MOTOR_B1_Pin GPIO_PIN_13
#define MOTOR_B1_GPIO_Port GPIOE
#define MOTOR_B2_Pin GPIO_PIN_15
#define MOTOR_B2_GPIO_Port GPIOE
#define ENCODER_B1_Pin GPIO_PIN_15
#define ENCODER_B1_GPIO_Port GPIOA
#define ENCODER_B2_Pin GPIO_PIN_3
#define ENCODER_B2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
