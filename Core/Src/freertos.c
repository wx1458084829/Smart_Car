/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "car_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Task_200HZHandle;
osThreadId Task_100HZHandle;
osThreadId Task_PrintfHandle;
osThreadId Task_InteractioHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask_200HZ(void const * argument);
void StartTask_100HZ(void const * argument);
void StartTask_Printf(void const * argument);
void StartTask_Interaction(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_200HZ */
  osThreadDef(Task_200HZ, StartTask_200HZ, osPriorityNormal, 0, 128);
  Task_200HZHandle = osThreadCreate(osThread(Task_200HZ), NULL);

  /* definition and creation of Task_100HZ */
  osThreadDef(Task_100HZ, StartTask_100HZ, osPriorityIdle, 0, 128);
  Task_100HZHandle = osThreadCreate(osThread(Task_100HZ), NULL);

  /* definition and creation of Task_Printf */
  osThreadDef(Task_Printf, StartTask_Printf, osPriorityIdle, 0, 128);
  Task_PrintfHandle = osThreadCreate(osThread(Task_Printf), NULL);

  /* definition and creation of Task_Interactio */
  osThreadDef(Task_Interactio, StartTask_Interaction, osPriorityIdle, 0, 128);
  Task_InteractioHandle = osThreadCreate(osThread(Task_Interactio), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTask_200HZ */
/**
  * @brief  Function implementing the Task_200HZ thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_200HZ */
void StartTask_200HZ(void const * argument)
{
  /* USER CODE BEGIN StartTask_200HZ */
	MPU_Init();
	while(mpu_dmp_init());
	
//	struct _acc acc;
//	struct _gyro gyro;
	
  /* Infinite loop */
  for(;;)
  {
//		MPU_Get_Gyroscope(&acc.x,&acc.y,&acc.z);
//		MPU_Get_Accelerometer(&gyro.x,&gyro.y,&gyro.z);
//		printf("%d,%d,%d -- \r\n",acc.x,acc.y,acc.z);
		//printf("%d,%d,%d -- \r\n",gyro.x,gyro.y,gyro.z);
		Car_Task_200HZ();
    osDelay(5);
  }
  /* USER CODE END StartTask_200HZ */
}

/* USER CODE BEGIN Header_StartTask_100HZ */
/**
* @brief Function implementing the Task_100HZ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_100HZ */
void StartTask_100HZ(void const * argument)
{
  /* USER CODE BEGIN StartTask_100HZ */
  /* Infinite loop */
	
	//Set_PWM(0,4000);
  for(;;)
  {
		Car_Task_100HZ();
    osDelay(10);
  }
  /* USER CODE END StartTask_100HZ */
}

/* USER CODE BEGIN Header_StartTask_Printf */
/**
* @brief Function implementing the Task_Printf thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Printf */
void StartTask_Printf(void const * argument)
{
  /* USER CODE BEGIN StartTask_Printf */
  /* Infinite loop */
  for(;;)
  {
		
		Printf_Task();
    osDelay(200);
  }
  /* USER CODE END StartTask_Printf */
}

/* USER CODE BEGIN Header_StartTask_Interaction */
/**
* @brief Function implementing the Task_Interactio thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Interaction */
void StartTask_Interaction(void const * argument)
{
  /* USER CODE BEGIN StartTask_Interaction */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);	//100hz
  }
  /* USER CODE END StartTask_Interaction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
