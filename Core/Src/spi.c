/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (spiHandle->Instance == SPI1)
  {
    /* USER CODE BEGIN SPI1_MspInit 0 */

    /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_MspInit 1 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

  if (spiHandle->Instance == SPI1)
  {
    /* USER CODE BEGIN SPI1_MspDeInit 0 */

    /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    /* USER CODE BEGIN SPI1_MspDeInit 1 */

    /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Erase flash sector
  * @param  sectorAddr: Sector to be erased
  * @retval None
  */
void SPI_FLASH_SectorErase(uint32_t sectorAddr)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(W25X_SectorErase);
  SPI_FLASH_SendByte((sectorAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((sectorAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(sectorAddr & 0xFF);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param  pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval None
  */
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);
  /*发送写地址的高位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
    NumByteToWrite = SPI_FLASH_PerWritePageSize;
    FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param  pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval None
  */
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;

  /*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;
  /*计算出要写多少整数页*/
  NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
  /*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr += SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      /*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
    /* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
      /*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingle > count)
      {
        temp = NumOfSingle - count;

        /*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr += count;
        pBuffer += count;

        /*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr += count;
      pBuffer += count;

      /*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr += SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
      /*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief   读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval  None
  */
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送 读 指令 */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* 发送 读 地址高位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /* 发送 读 地址低位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  /* 读取数据 */
  while (NumByteToRead--)
  {
    /* 读取一个字节*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  读取FLASH ID
  * @param  None
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH();

  /*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  读取FLASH Device ID
  * @param  None
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}

/**
  * @name   SPI_FLASH_StartReadSequence
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *         This is done by driving the /CS line low to select the device,
  *         then the READ instruction is transmitted followed by 3 bytes
  *         address. This function exit and keep the /CS line low, so the
  *         Flash still being selected. With this technique the whole
  *         content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(0);
  }

  WRITE_REG(hspi1.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(1);
  }

  return READ_REG(hspi1.Instance->DR);
}

/**
  * @name   SPI_FLASH_SendHalfWord
  * @brief  Sends a Half Word through the SPI interface and return the
  *         Half Word received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(2);
  }

  /* Send Half Word through the SPIx peripheral */
  WRITE_REG(hspi1.Instance->DR, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(3);
  }
  /* Return the Half Word read from the SPI bus */
  return READ_REG(hspi1.Instance->DR);
}

/**
  * @brief  向FLASH发送 写使能 命令
  * @param  None
  * @retval None
  */
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(W25X_WriteEnable);
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  None
  * @retval None
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(W25X_ReadStatusReg);
  SPITimeout = SPIT_FLAG_TIMEOUT;

  do
  {
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

    {
      if ((SPITimeout--) == 0)
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    }
  } while ((FLASH_Status & WIP_Flag) == SET);

  SPI_FLASH_CS_HIGH();
}

//进入掉电模式
void SPI_Flash_PowerDown(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(W25X_PowerDown);
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  Wait timeout callback function
  * @param  None
  * @retval None
  */
void SPI_Flash_WAKE_UP(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  Wait timeout callback function
  * @param  errorCode: A number points out which error occurs
  * @retval None
  */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  FLASH_ERROR("SPI等待超时!errorCode = %d", errorCode);
  return 0;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
