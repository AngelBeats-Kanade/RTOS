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
#include "touch_screen.h"
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
osThreadId defaultTaskHandle;
osThreadId LED1TaskHandle;
osThreadId LCDTask01Handle;
osThreadId LCDTask02Handle;
osThreadId BeepTaskHandle;
osThreadId LED2TaskHandle;
osThreadId TPADTaskHandle;
osThreadId LCDTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);

void LED1_Task(void const *argument);

void LCD_Task1(void const *argument);

void LCD_Task2(void const *argument);

void Beep_Task(void const *argument);

void LED2_Task(void const *argument);

void TPAD_Task(void const *argument);

void LCD_Task3(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED1Task */
  osThreadDef(LED1Task, LED1_Task, osPriorityLow, 0, 128);
  LED1TaskHandle = osThreadCreate(osThread(LED1Task), NULL);

  /* definition and creation of LCDTask01 */
  osThreadDef(LCDTask01, LCD_Task1, osPriorityRealtime + 1, 0, 128);
  LCDTask01Handle = osThreadCreate(osThread(LCDTask01), NULL);

  /* definition and creation of LCDTask02 */
  osThreadDef(LCDTask02, LCD_Task2, osPriorityRealtime + 2, 0, 128);
  LCDTask02Handle = osThreadCreate(osThread(LCDTask02), NULL);

  /* definition and creation of BeepTask */
  osThreadDef(BeepTask, Beep_Task, osPriorityRealtime, 0, 128);
  BeepTaskHandle = osThreadCreate(osThread(BeepTask), NULL);

  /* definition and creation of LED2Task */
  osThreadDef(LED2Task, LED2_Task, osPriorityLow, 0, 128);
  LED2TaskHandle = osThreadCreate(osThread(LED2Task), NULL);

  /* definition and creation of TPADTask */
  osThreadDef(TPADTask, TPAD_Task, osPriorityRealtime, 0, 128);
  TPADTaskHandle = osThreadCreate(osThread(TPADTask), NULL);

  /* definition and creation of LCDTask03 */
  osThreadDef(LCDTask03, LCD_Task3, osPriorityRealtime + 3, 0, 128);
  LCDTask03Handle = osThreadCreate(osThread(LCDTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
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
void LED1_Task(void const *argument)
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
void LCD_Task1(void const *argument)
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
void LCD_Task2(void const *argument)
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
void Beep_Task(void const *argument)
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
void LED2_Task(void const *argument)
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
void TPAD_Task(void const *argument)
{
  /* USER CODE BEGIN TPAD_Task */
  char TPAD_flag = 0;
  /* Infinite loop */
  for (;;)
  {
    if (TPAD_Scan(1))
    {
      ILI9341_DispStringLine_EN_CH(LINE(14), "TPAD touched!");
      TPAD_flag = 1;
    }
    else
    {
      if (TPAD_flag == 1)
      {
        LCD_ClearLine(LINE(13));
        LCD_ClearLine(LINE(14));
        TPAD_flag = 0;
      }
    }
    osDelay(100);
  }
  /* USER CODE END TPAD_Task */
}

/* USER CODE BEGIN Header_LCD_Task3 */
/**
* @brief Function implementing the LCDTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LCD_Task3 */
void LCD_Task3(void const *argument)
{
  /* USER CODE BEGIN LCD_Task3 */
  /* Infinite loop */
  for (;;)
  {
    XPT2046_TouchEvenHandler();
    osDelay(10);
  }
  /* USER CODE END LCD_Task3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
