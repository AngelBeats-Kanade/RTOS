/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usart.h"
#include "lcd.h"
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
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED1_Task_Handle;

static StackType_t AppTaskCreate_Stack[128];
static StackType_t LED1_Task_Stack[128];

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED1_Task_TCB;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void AppTaskCreate(void);

static void LED1_Task(void *pvParameters);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t) AppTaskCreate,
                                           (const char *) "AppTaskCreate",
                                           (uint32_t) 128,
                                           (void *) NULL,
                                           (UBaseType_t) 3,
                                           (StackType_t *) AppTaskCreate_Stack,
                                           (StaticTask_t *) &AppTaskCreate_TCB);
  if (NULL == AppTaskCreate_Handle)
  {
    Error_Handler();
  }
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief   为了方便管理，所有的任务创建函数都放在这个函数里面
  * @param   无
  * @retval  无
  * @author  AngelBeats
  * @version V1.0
  * @date    2020-12-27
  */
static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();

  LED1_Task_Handle = xTaskCreateStatic((TaskFunction_t) LED1_Task,
                                       (const char *) "LED1_Task",
                                       (uint32_t) 128,
                                       (void *) NULL,
                                       (UBaseType_t) 4,
                                       (StackType_t *) LED1_Task_Stack,
                                       (StaticTask_t *) &LED1_Task_TCB);

  if (NULL != LED1_Task_Handle)
    printf("LED1_Task任务创建成功!\n");
  else
    printf("LED1_Task任务创建失败!\n");

  vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();
}

/**
  * @brief   LED1的任务主体
  * @param   无
  * @retval  无
  * @author  AngelBeats
  * @version V1.0
  * @date    2020-12-27
  */
static void LED1_Task(void *paramter)
{
  while (1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    osDelay(500);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    osDelay(500);
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
