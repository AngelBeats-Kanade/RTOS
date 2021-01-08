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

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  if (retUSER != 0)
  {
    Error_Handler();
  }
  /* USER CODE END Init */
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
