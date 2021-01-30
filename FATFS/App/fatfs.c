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
UINT f_num;
BYTE readBuffer[1024] = {0};
BYTE writeBuffer[] = "新建文件系统测试文件!\n";

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

    if (f_res == FR_NO_FILESYSTEM)
    {
      f_res = f_mkfs((TCHAR const *) USERPath, 0, 0);

      if (f_res == FR_OK)
      {
        f_res = f_mount(NULL, (TCHAR const *) USERPath, 1);
        f_res = f_mount(&USERFatFS, (TCHAR const *) USERPath, 1);
      }
      else
      {
        printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
        Error_Handler();
      }
    }
    else if (f_res != FR_OK)
    {
      printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
      printf_fatfs_error(f_res);
      Error_Handler();
    }

    fatfs_write_test();

    fatfs_read_test();

    FATFS_UnLinkDriver(USERPath);
  }
  else
  {
    printf("Hardware error!\n");
    printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
    FATFS_UnLinkDriver(USERPath);
  }
  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */
/**
  * @brief  调试信息
  * @param  无
  * @retval 无
  */
void printf_fatfs_error(FRESULT f_result)
{
  switch (f_result)
  {
    case FR_OK:
      printf("》操作成功。\r\n");
      break;
    case FR_DISK_ERR:
      printf("！！硬件输入输出驱动出错。\r\n");
      break;
    case FR_INT_ERR:
      printf("！！断言错误。\r\n");
      break;
    case FR_NOT_READY:
      printf("！！物理设备无法工作。\r\n");
      break;
    case FR_NO_FILE:
      printf("！！无法找到文件。\r\n");
      break;
    case FR_NO_PATH:
      printf("！！无法找到路径。\r\n");
      break;
    case FR_INVALID_NAME:
      printf("！！无效的路径名。\r\n");
      break;
    case FR_DENIED:
    case FR_EXIST:
      printf("！！拒绝访问。\r\n");
      break;
    case FR_INVALID_OBJECT:
      printf("！！无效的文件或路径。\r\n");
      break;
    case FR_WRITE_PROTECTED:
      printf("！！逻辑设备写保护。\r\n");
      break;
    case FR_INVALID_DRIVE:
      printf("！！无效的逻辑设备。\r\n");
      break;
    case FR_NOT_ENABLED:
      printf("！！无效的工作区。\r\n");
      break;
    case FR_NO_FILESYSTEM:
      printf("！！无效的文件系统。\r\n");
      break;
    case FR_MKFS_ABORTED:
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
      break;
    case FR_TIMEOUT:
      printf("！！操作超时。\r\n");
      break;
    case FR_LOCKED:
      printf("！！文件被保护。\r\n");
      break;
    case FR_NOT_ENOUGH_CORE:
      printf("！！长文件名支持获取堆空间失败。\r\n");
      break;
    case FR_TOO_MANY_OPEN_FILES:
      printf("！！打开太多文件。\r\n");
      break;
    case FR_INVALID_PARAMETER:
      printf("！！参数无效。\r\n");
      break;
  }
}

void fatfs_read_test(void)
{
  f_res = f_open(&USERFile, "1.txt", FA_OPEN_ALWAYS | FA_READ);

  if (f_res == FR_OK)
  {
    f_res = f_read(&USERFile, readBuffer, sizeof(readBuffer), &f_num);

    if (f_res == FR_OK)
    {
      printf("Number read: %d\t", f_num);
      printf("String read: %s\n", readBuffer);
    }
    else
    {
      printf_fatfs_error(f_res);
      printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
    }
  }
  else
  {
    printf_fatfs_error(f_res);
    printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
  }

  f_close(&USERFile);

  f_res = f_mount(NULL, (TCHAR const *) USERPath, 1);

  if (f_res != FR_OK)
  {
    printf_fatfs_error(f_res);
    printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
  }
}

void fatfs_write_test(void)
{
  f_res = f_open(&USERFile, "1.txt", FA_CREATE_ALWAYS | FA_WRITE);

  if (f_res == FR_OK)
  {
    f_res = f_write(&USERFile, writeBuffer, sizeof(writeBuffer), &f_num);

    if (f_res == FR_OK)
    {
      printf("Number write: %d\t", f_num);
      printf("String write: %s\n", writeBuffer);
    }
    else
    {
      printf_fatfs_error(f_res);
      printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
    }
    f_close(&USERFile);
  }
  else
  {
    printf_fatfs_error(f_res);
    printf("Wrong parameters value: file %s on line %d\n", __FILE__, __LINE__);
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
