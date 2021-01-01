/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "spi.h"
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
uint8_t retUSER;  /* Return value for USER */
char USERPath[4]; /* USER logical drive path */
FATFS USERFatFS;  /* File system object for USER logical drive */
FIL USERFile;     /* File object for USER */
/* USER CODE BEGIN Variables */
static volatile FRESULT res;
UINT USERNum;
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "\nSPI-FatFs文件系统\n";
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  FatFs initialization
  * @param  None
  * @retval None
  */
void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  if (retUSER == 0)
  {
    res = f_mount(&USERFatFS, USERPath, 1);

    if (res == FR_NO_FILESYSTEM)
    {
      printf("\nFLASH还没有文件系统，即将进行格式化！\n");

      res = f_mkfs(USERPath, 0, 0);
      
      if (res == FR_OK)
      {
        printf("\nFLASH已成功格式化文件系统！\n");
        res = f_mount(NULL, USERPath, 1);
        res = f_mount(&USERFatFS, USERPath, 1);
      }
      else
      {
        printf("\n格式化失败！\n");
        Error_Handler();
      }
    }
    else if (res!=FR_OK)
    {
      printf("\n！！外部FLASH挂载文件系统失败！！(%d)\n", res);
      printf("\n！！可能原因：SPI FLASH初始化不成功！！\n");
      Error_Handler();
    }
    else
    {
      printf("\n文件系统挂载成功！！\n");
    }
  }
  else
  {
    printf("\n！！文件系统未能与FLASH建立联系！！\n");
    Error_Handler();
  }
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
/**
  * @brief   File system read and write test.
  * @param   None
  * @retval  None
  * @author  AngelBeats
  * @version V1.0
  * @date    2021-1-2
  */
void FileSystemTest(void)
{ 
  printf("\n<-------即将进行文件写入测试...------->\n");
  res = f_open(&USERFile, "1:FatFs读写测试文件.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK)
  {
    printf(">打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\n");
    res = f_write(&USERFile, WriteBuffer, sizeof(WriteBuffer), &USERNum);
    if (res == FR_OK)
    {
      printf(">文件写入成功，写入字节数据：%d。\n", USERNum);
      printf(">向文件写入的数据为：\r\n%s。\r\n", WriteBuffer);
    }
    else
    {
      printf(">文件写入失败：(%d)！！\n", res);
    }
    f_close(&USERFile);
  }
  else
  {
    printf("打开/创建文件失败！！\n");
    printf("error code = %d\n", res);
  }

  printf("\n<-------即将进行文件读取测试...------->\n");
  res = f_open(&USERFile, "1:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ);
  if (res == FR_OK)
  {
    printf(">打开文件成功。\n");
    res = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &USERNum);
    if (res == FR_OK)
    {
      printf(">文件读取成功,读到字节数据：%d。\n", USERNum);
      printf(">读取得的文件数据为：\r\n%s。\r\n", ReadBuffer);
    }
    else 
    {
      printf(">文件读取失败：(%d)！！\n", res);
    }
  } 
  else
  {
    printf("打开文件失败！！\n");
    printf("error code = %d\n", res);
  }

  f_close(&USERFile);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
