/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
 * @brief Ϊ���ݰ����ŵ磬�����ʱ����־������
 * @param ��
 * @retval ��
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
 * @brief �õ���ʱ������ֵ�������ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ
 * @param ��
 * @retval ��ʱ���Ĳ���ֵ
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
 * @brief ��ȡn�Σ�ȡ���ֵ
 * @param n��������ȡ�Ĵ���
 * @retval res�����������ж�����������ֵ
 */
static uint16_t TPAD_Get_MaxVal(uint8_t n) {
  uint16_t temp = 0;
  uint16_t res = 0;
  while (n--) {
    temp = TPAD_Get_Val(); //�õ�һ��ֵ
    if (temp > res)
      res = temp;
  };
  return res;
}

/**
 * @brief ��ʼ��������������ȡ����ʱ����������ȡֵ
 * @param ��
 * @retval 0����ʼ���ɹ���1����ʼ��ʧ��
 */
uint8_t TPAD_Init(void) {
  uint16_t buf[10];
  uint32_t temp = 0;
  uint8_t j, i;

  /* ��1.5Mhz��Ƶ�ʼ���  */
  MX_TIM5_Init();
  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

  /* ������ȡ10�� */
  for (i = 0; i < 10; i++) {
    buf[i] = TPAD_Get_Val();
    HAL_Delay(10);
  }
  /* ���� */
  for (i = 0; i < 9; i++) {
    for (j = i + 1; j < 10; j++) {
      /* �������� */
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  temp = 0;
  /* ȡ�м��6�����ݽ���ƽ�� */
  for (i = 2; i < 8; i++)
    temp += buf[i];
  tpad_default_val = temp / 6;
  printf("tpad_default_val:%d\r\n", tpad_default_val);
  /* ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������! */
  if (tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    return 1;
  return 0;
}

/**
 * @brief ɨ�败������
 * @param mode: 0����֧����������(����һ�α����ɿ����ܰ���һ��)��1��֧����������
 * @retval res��0��û�а��£�1���а���
 */
uint8_t TPAD_Scan(uint8_t mode) {
  // 0,���Կ�ʼ���;>0,�����ܿ�ʼ���
  static uint8_t keyen = 0;
  //ɨ����
  uint8_t res = 0;
  //Ĭ�ϲ�������Ϊ3��
  uint8_t sample = 3;
  //����ֵ
  uint16_t rval;

  if (mode) {
    //֧��������ʱ�����ò�������Ϊ6��
    sample = 6;
    //֧������
    keyen = 0;
  }
  /* ��ȡ��ǰ����ֵ(���� sample ��ɨ������ֵ) */
  rval = TPAD_Get_MaxVal(sample);
  /* printf��ӡ��������ʹ�ã�����ȷ����ֵTPAD_GATE_VAL����Ӧ�ù�����Ӧע�͵� */
  // printf("scan_rval=%d\n", rval);

  //����tpad_default_val+TPAD_GATE_VAL,��С��10��tpad_default_val,����Ч
  if (rval > (tpad_default_val + TPAD_GATE_VAL) &&
      rval < (10 * tpad_default_val)) {
    // keyen==0,��Ч
    if (keyen == 0) {
      res = 1;
    }
    keyen = 3; //����Ҫ�ٹ�3��֮����ܰ�����Ч
  }

  if (keyen) {
    keyen--;
  }
  return res;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
