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
#include "tim.h"
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
extern __IO uint16_t Current_Temperature;
extern uint16_t ADC_ConvertedValue[2];
char buff0[30];
char buff1[30];
char flag;
char everDisplay = 0;
__IO float adcValue;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for LED1Task */
osThreadId_t LED1TaskHandle;
const osThreadAttr_t LED1Task_attributes = {
  .name = "LED1Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for LCDTask01 */
osThreadId_t LCDTask01Handle;
const osThreadAttr_t LCDTask01_attributes = {
  .name = "LCDTask01",
  .priority = (osPriority_t) osPriorityRealtime1,
  .stack_size = 128 * 4
};
/* Definitions for LCDTask02 */
osThreadId_t LCDTask02Handle;
const osThreadAttr_t LCDTask02_attributes = {
  .name = "LCDTask02",
  .priority = (osPriority_t) osPriorityRealtime2,
  .stack_size = 128 * 4
};
/* Definitions for BeepTask */
osThreadId_t BeepTaskHandle;
const osThreadAttr_t BeepTask_attributes = {
  .name = "BeepTask",
  .priority = (osPriority_t) osPriorityRealtime7,
  .stack_size = 128 * 4
};
/* Definitions for LED2Task */
osThreadId_t LED2TaskHandle;
const osThreadAttr_t LED2Task_attributes = {
  .name = "LED2Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for TPADTask */
osThreadId_t TPADTaskHandle;
const osThreadAttr_t TPADTask_attributes = {
  .name = "TPADTask",
  .priority = (osPriority_t) osPriorityRealtime6,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LED1_Task(void *argument);
void LCD_Task1(void *argument);
void LCD_Task2(void *argument);
void Beep_Task(void *argument);
void LED2_Task(void *argument);
void TPAD_Task(void *argument);

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

  /* creation of LED1Task */
  LED1TaskHandle = osThreadNew(LED1_Task, NULL, &LED1Task_attributes);

  /* creation of LCDTask01 */
  LCDTask01Handle = osThreadNew(LCD_Task1, NULL, &LCDTask01_attributes);

  /* creation of LCDTask02 */
  LCDTask02Handle = osThreadNew(LCD_Task2, NULL, &LCDTask02_attributes);

  /* creation of BeepTask */
  BeepTaskHandle = osThreadNew(Beep_Task, NULL, &BeepTask_attributes);

  /* creation of LED2Task */
  LED2TaskHandle = osThreadNew(LED2_Task, NULL, &LED2Task_attributes);

  /* creation of TPADTask */
  TPADTaskHandle = osThreadNew(TPAD_Task, NULL, &TPADTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

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

/* USER CODE BEGIN Header_LED1_Task */
/**
  * @brief   Function implementing the LED1Task thread.
  * @param   argument: Not used
  * @retval  None
  * @author  AngelBeats
  * @version V1.0
  * @date    2020-12-30
  */
/* USER CODE END Header_LED1_Task */
void LED1_Task(void *argument)
{
  /* USER CODE BEGIN LED1_Task */
  /* Infinite loop */
  for (;;)
  {
    if (flag == 0)
    {
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }
    else
    {
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    }
    osDelay(500);
  }
  /* USER CODE END LED1_Task */
}

/* USER CODE BEGIN Header_LCD_Task1 */
/**
  * @brief   Function implementing the LCDTask01 thread.
  * @param   argument: Not used
  * @retval  None
  * @author  AngelBeats
  * @version V1.0
  * @date    2020-12-30
  */
/* USER CODE END Header_LCD_Task1 */
void LCD_Task1(void *argument)
{
  /* USER CODE BEGIN LCD_Task1 */
  /* Infinite loop */
  for (;;)
  {
    Current_Temperature = (1774 - ADC_ConvertedValue[0]) / 5 + 25;
    sprintf(buff0, "Current temperature: %3d `c", Current_Temperature);
    ILI9341_DispStringLine_EN(LINE(10), buff0);

    osDelay(200);
  }
  /* USER CODE END LCD_Task1 */
}

/* USER CODE BEGIN Header_LCD_Task2 */
/**
  * @brief   Function implementing the LCDTask02 thread.
  * @param   argument: Not used
  * @retval  None
  * @version V1.0
  * @date    2020-12-30
  * @note    Traditional printf and sprintf cost too many
  *          stacks to process float numbers. So I choose
  *          turning float to int. If using printf
  *          specially optimized fot embedded project,
  *          it can be avoided.
  */
/* USER CODE END Header_LCD_Task2 */
void LCD_Task2(void *argument)
{
  /* USER CODE BEGIN LCD_Task2 */
  /* Infinite loop */
  for (;;)
  {
    adcValue = (float) ADC_ConvertedValue[1] / 4096 * 3.3;
    uint16_t units_place, tenth, hundredth, thousandth, ten_thousandth;
    units_place = (uint16_t) adcValue;
    tenth = (uint16_t) (10 * (adcValue - units_place));
    hundredth = (uint16_t) (100 * (adcValue - units_place) - 10 * tenth);
    thousandth = (uint16_t) (1000 * (adcValue - units_place) - 100 * tenth - 10 * hundredth);
    ten_thousandth = (uint16_t) (10000 * (adcValue - units_place) - 1000 * tenth - 100 * hundredth - 10 * thousandth);
    sprintf(buff1, "Current voltage: %1d.%1d%1d%1d%1d V", units_place, tenth, hundredth, thousandth, ten_thousandth);
    ILI9341_DispStringLine_EN(LINE(11), buff1);

    osDelay(200);
  }
  /* USER CODE END LCD_Task2 */
}

/* USER CODE BEGIN Header_Beep_Task */
/**
  * @brief   Function implementing the BeepTask thread.
  * @param   argument: Not used
  * @retval  None
  * @version V1.0
  * @date    2020-12-30
  */
/* USER CODE END Header_Beep_Task */
void Beep_Task(void *argument)
{
  /* USER CODE BEGIN Beep_Task */
  /* Infinite loop */
  for (;;)
  {
    if (adcValue >= 3.0)
    {
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
      if (everDisplay == 0)
      {
        everDisplay = 1;
        LCD_ClearLine(LINE(12));
        LCD_ClearLine(LINE(13));
        LCD_ClearLine(LINE(14));
      }
      ILI9341_DispStringLine_EN_CH(LINE(13), "Danger! Danger! Danger!");
    }
    else if (adcValue >= 2.0)
    {
      flag = 1;
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
      if (everDisplay == 1)
      {
        everDisplay = 0;
        LCD_ClearLine(LINE(12));
        LCD_ClearLine(LINE(13));
        LCD_ClearLine(LINE(14));
      }
      ILI9341_DispStringLine_EN_CH(LINE(13), "Alarm! Voltage too high!");
    }
    else
    {
      flag = 0;
      if (everDisplay == 0)
      {
        everDisplay = 1;
        LCD_ClearLine(LINE(13));
      }
    }

    osDelay(100);
  }
  /* USER CODE END Beep_Task */
}

/* USER CODE BEGIN Header_LED2_Task */
/**
  * @brief Function implementing the LED2Task thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED2_Task */
void LED2_Task(void *argument)
{
  /* USER CODE BEGIN LED2_Task */
  /* Infinite loop */
  for (;;)
  {
    if (flag == 1)
    {
      HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }
    else
    {
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    }
    osDelay(200);
  }
  /* USER CODE END LED2_Task */
}

/* USER CODE BEGIN Header_TPAD_Task */
/**
  * @brief Function implementing the TPADTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_TPAD_Task */
void TPAD_Task(void *argument)
{
  /* USER CODE BEGIN TPAD_Task */
  /* Infinite loop */
  for (;;)
  {
    char TPAD_flag = 0;
    if (TPAD_Scan(0))
    {
      ILI9341_DispStringLine_EN_CH(LINE(14), "TPAD touched!");
      TPAD_flag = 1;
    }
    osDelay(100);
    if (TPAD_flag == 1)
    {
      TPAD_flag = 0;
      LCD_ClearLine(LINE(13));
      LCD_ClearLine(LINE(14));
    }
  }
  /* USER CODE END TPAD_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
