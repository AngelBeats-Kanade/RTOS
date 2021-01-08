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

#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
FRESULT f_res;
UINT fnum;
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "Welcome to use stm32, this is a file system test file.\r\n";

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  if (retUSER == 0)
  {
    f_res = f_mount(&USERFatFS, (TCHAR const *) USERPath, 1);
    printf_fatfs_error(f_res);
    /*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (f_res == FR_NO_FILESYSTEM)
    {
      printf(">>There is no filesystem in flash, initializing filesystem...\r\n");
      /* 格式化 */
      f_res = f_mkfs((TCHAR const *) USERPath, 0, 0);

      if (f_res == FR_OK)
      {
        printf(">>Initialize successfully!\r\n");
        /* 格式化后，先取消挂载 */
        f_res = f_mount(NULL, (TCHAR const *) USERPath, 1);
        /* 重新挂载	*/
        f_res = f_mount(&USERFatFS, (TCHAR const *) USERPath, 1);
      }
      else
      {
        printf(">>Initialization error!\r\n");
        Error_Handler();
      }
    }
    else if (f_res != FR_OK)
    {
      printf(">>Fail to mount filesystem: (%d)\r\n", f_res);
      printf_fatfs_error(f_res);
      Error_Handler();
    }
    else
    {
      printf(">>Mount filesystem successfully!\r\n");
    }

    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("<-------Write test... ------->\r\n");
    f_res = f_open(&USERFile, "FatFs", FA_CREATE_ALWAYS | FA_WRITE);
    if (f_res == FR_OK)
    {
      printf(">>Open/create FatFs.txt successfully, write data to it.\r\n");
      /* 将指定存储区内容写入到文件内 */
      f_res = f_write(&USERFile, WriteBuffer, sizeof(WriteBuffer), &fnum);
      if (f_res == FR_OK)
      {
        printf(">>Write successfully, byte data written: %d\r\n", fnum);
        printf(">>Data written to file: \r\n%s\r\n", WriteBuffer);
      }
      else
      {
        printf(">>Fail to write file: (%d)\r\n", f_res);
      }
      /* 不再读写，关闭文件 */
      f_close(&USERFile);
    }
    else
    {
      printf(">>Fail to open/create file!\r\n");
    }

    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("<-------Read test... ------->\r\n");
    f_res = f_open(&USERFile, "FatFs", FA_OPEN_EXISTING | FA_READ);
    if (f_res == FR_OK)
    {
      printf(">>Open file success!\r\n");
      f_res = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
      if (f_res == FR_OK)
      {
        printf(">>Read file success, byte data read: %d\r\n", fnum);
        printf(">>Data read:\r\n%s \r\n", ReadBuffer);
      }
      else
      {
        printf(">>Fail to read file: (%d)\r\n", f_res);
      }
    }
    else
    {
      printf(">>Fail to open file!\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&USERFile);

    /* 不再使用，取消挂载 */
    f_res = f_mount(NULL, (TCHAR const *) USERPath, 1);
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
  * @brief  调试信息
  * @param  无
  * @retval 无
  */
void printf_fatfs_error(FRESULT fresult)
{
  switch (fresult)
  {
    case FR_OK:
      printf(">>Success!\r\n");
      break;
    case FR_DISK_ERR:
      printf(">>Hardware I/O driver error!\r\n");
      break;
    case FR_INT_ERR:
      printf(">>Assert error!\r\n");
      break;
    case FR_NOT_READY:
      printf(">>Hardware device can`t work!\r\n");
      break;
    case FR_NO_FILE:
      printf(">>Unable to find file!\r\n");
      break;
    case FR_NO_PATH:
      printf(">>Unable to find path!\r\n");
      break;
    case FR_INVALID_NAME:
      printf(">>Invalid file name!\r\n");
      break;
    case FR_DENIED:
    case FR_EXIST:
      printf(">>Read denied!\r\n");
      break;
    case FR_INVALID_OBJECT:
      printf(">>Invalid file or file path!\r\n");
      break;
    case FR_WRITE_PROTECTED:
      printf(">>Logic device write protected!\r\n");
      break;
    case FR_INVALID_DRIVE:
      printf(">>Invalid logic device!\r\n");
      break;
    case FR_NOT_ENABLED:
      printf(">>Invalid workspace!\r\n");
      break;
    case FR_NO_FILESYSTEM:
      printf(">>Invalid filesystem!\r\n");
      break;
    case FR_MKFS_ABORTED:
      printf(">>f_mkfs error!\r\n");
      break;
    case FR_TIMEOUT:
      printf(">>Time out!\r\n");
      break;
    case FR_LOCKED:
      printf(">>File is locked!\r\n");
      break;
    case FR_NOT_ENOUGH_CORE:
      printf(">>Long file name support gets stack fail!\r\n");
      break;
    case FR_TOO_MANY_OPEN_FILES:
      printf(">>Open too many files!\r\n");
      break;
    case FR_INVALID_PARAMETER:
      printf(">>Invalid parameter!\r\n");
      break;
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
