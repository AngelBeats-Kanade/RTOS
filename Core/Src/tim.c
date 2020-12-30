/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#define TPAD_ARR_MAX_VAL 0XFFFF
#define TPAD_GATE_VAL 100

__IO uint16_t tpad_default_val = 0;
/* USER CODE END 0 */

TIM_HandleTypeDef htim5;

/* TIM5 init function */
void MX_TIM5_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 47;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 3;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM5 GPIO Configuration
    PA1     ------> TIM5_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();

    /**TIM5 GPIO Configuration
    PA1     ------> TIM5_CH2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief 为电容按键放电，清除定时器标志及计数
 * @param 无
 * @retval 无
 */
static void TPAD_Reset(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_Delay(5);

  __HAL_TIM_SET_COUNTER(&htim5, 0);
  __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE | TIM_FLAG_CC2);

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
}

/**
 * @brief 得到定时器捕获值，如果超时,则直接返回定时器的计数值
 * @param 无
 * @retval 定时器的捕获值
 */
static uint16_t TPAD_Get_Val(void) {
  TPAD_Reset();
  while (__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_CC2) == RESET) {
    uint16_t count;
    count = __HAL_TIM_GET_COUNTER(&htim5);
    if (count > (0xFFFF - 500))
      return count;
  }
  return HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_2);
}

/**
 * @brief 读取n次，取最大值
 * @param n：连续获取的次数
 * @retval res：连续读数中读到的最大读数值
 */
static uint16_t TPAD_Get_MaxVal(uint8_t n) {
  uint16_t temp = 0;
  uint16_t res = 0;
  while (n--) {
    temp = TPAD_Get_Val(); //得到一次值
    if (temp > res)
      res = temp;
  };
  return res;
}

/**
 * @brief 初始化触摸按键，获取空载时触摸按键的取值
 * @param 无
 * @retval 0：初始化成功；1：初始化失败
 */
uint8_t TPAD_Init(void) {
  uint16_t buf[10];
  uint32_t temp = 0;
  uint8_t j, i;

  /* 以1.5Mhz的频率计数  */
  MX_TIM5_Init();
  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

  /* 连续读取10次 */
  for (i = 0; i < 10; i++) {
    buf[i] = TPAD_Get_Val();
    HAL_Delay(10);
  }
  /* 排序 */
  for (i = 0; i < 9; i++) {
    for (j = i + 1; j < 10; j++) {
      /* 升序排列 */
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  temp = 0;
  /* 取中间的6个数据进行平均 */
  for (i = 2; i < 8; i++)
    temp += buf[i];
  tpad_default_val = temp / 6;
  printf("tpad_default_val:%d\r\n", tpad_default_val);
  /* 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常! */
  if (tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    return 1;
  return 0;
}

/**
 * @brief 扫描触摸按键
 * @param mode: 0，不支持连续触发(按下一次必须松开才能按下一次)；1，支持连续触发
 * @retval res：0，没有按下；1，有按下
 */
uint8_t TPAD_Scan(uint8_t mode) {
  // 0,可以开始检测;>0,还不能开始检测
  static uint8_t keyen = 0;
  //扫描结果
  uint8_t res = 0;
  //默认采样次数为3次
  uint8_t sample = 3;
  //捕获值
  uint16_t rval;

  if (mode) {
    //支持连按的时候，设置采样次数为6次
    sample = 6;
    //支持连按
    keyen = 0;
  }
  /* 获取当前捕获值(返回 sample 次扫描的最大值) */
  rval = TPAD_Get_MaxVal(sample);
  /* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
  // printf("scan_rval=%d\n", rval);

  //大于tpad_default_val+TPAD_GATE_VAL,且小于10倍tpad_default_val,则有效
  if (rval > (tpad_default_val + TPAD_GATE_VAL) &&
      rval < (10 * tpad_default_val)) {
    // keyen==0,有效
    if (keyen == 0) {
      res = 1;
    }
    keyen = 3; //至少要再过3次之后才能按键有效
  }

  if (keyen) {
    keyen--;
  }
  return res;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
