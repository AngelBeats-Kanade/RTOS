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
BYTE WriteBuffer[] = "\nSPI-FatFs�ļ�ϵͳ\n";
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
      printf("\nFLASH��û���ļ�ϵͳ���������и�ʽ����\n");

      res = f_mkfs(USERPath, 0, 0);
      
      if (res == FR_OK)
      {
        printf("\nFLASH�ѳɹ���ʽ���ļ�ϵͳ��\n");
        res = f_mount(NULL, USERPath, 1);
        res = f_mount(&USERFatFS, USERPath, 1);
      }
      else
      {
        printf("\n��ʽ��ʧ�ܣ�\n");
        Error_Handler();
      }
    }
    else if (res!=FR_OK)
    {
      printf("\n�����ⲿFLASH�����ļ�ϵͳʧ�ܣ���(%d)\n", res);
      printf("\n��������ԭ��SPI FLASH��ʼ�����ɹ�����\n");
      Error_Handler();
    }
    else
    {
      printf("\n�ļ�ϵͳ���سɹ�����\n");
    }
  }
  else
  {
    printf("\n�����ļ�ϵͳδ����FLASH������ϵ����\n");
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
  printf("\n<-------���������ļ�д�����...------->\n");
  res = f_open(&USERFile, "1:FatFs��д�����ļ�.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK)
  {
    printf(">��/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\n");
    res = f_write(&USERFile, WriteBuffer, sizeof(WriteBuffer), &USERNum);
    if (res == FR_OK)
    {
      printf(">�ļ�д��ɹ���д���ֽ����ݣ�%d��\n", USERNum);
      printf(">���ļ�д�������Ϊ��\r\n%s��\r\n", WriteBuffer);
    }
    else
    {
      printf(">�ļ�д��ʧ�ܣ�(%d)����\n", res);
    }
    f_close(&USERFile);
  }
  else
  {
    printf("��/�����ļ�ʧ�ܣ���\n");
    printf("error code = %d\n", res);
  }

  printf("\n<-------���������ļ���ȡ����...------->\n");
  res = f_open(&USERFile, "1:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ);
  if (res == FR_OK)
  {
    printf(">���ļ��ɹ���\n");
    res = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &USERNum);
    if (res == FR_OK)
    {
      printf(">�ļ���ȡ�ɹ�,�����ֽ����ݣ�%d��\n", USERNum);
      printf(">��ȡ�õ��ļ�����Ϊ��\r\n%s��\r\n", ReadBuffer);
    }
    else 
    {
      printf(">�ļ���ȡʧ�ܣ�(%d)����\n", res);
    }
  } 
  else
  {
    printf("���ļ�ʧ�ܣ���\n");
    printf("error code = %d\n", res);
  }

  f_close(&USERFile);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
