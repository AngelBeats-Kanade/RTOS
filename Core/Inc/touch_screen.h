/**
  ******************************************************************************
  * @file    touch_screen.h
  * @brief   This file contains all the function prototypes for
  *          the touch_screen.c file
  * @date    2021/1/3
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 AngelBeats.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "spi.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* 声明 XPT2046 相关的数据类型 */

typedef struct //液晶坐标结构体
{
  /*负数值表示无新数据*/
  int16_t x;  //记录最新的触摸参数值
  int16_t y;

  /*用于记录连续触摸时(长按)的上一次触摸位置*/
  int16_t pre_x;
  int16_t pre_y;
} strType_XPT2046_Coordinate;


typedef struct //校准因子结构体
{
  float An,    //注:sizeof(long double) = 8
  Bn,
          Cn,
          Dn,
          En,
          Fn,
          Divider;
} strType_XPT2046_Calibration;


typedef struct //校准系数结构体（最终使用）
{
  float dX_X,
          dX_Y,
          dX,
          dY_X,
          dY_Y,
          dY;
} strType_XPT2046_TouchPara;

/* 触摸状态机相关 */
typedef enum {
  XPT2046_STATE_RELEASE = 0, //触摸释放
  XPT2046_STATE_WAITING,     //触摸按下
  XPT2046_STATE_PRESSED,     //触摸按下
} enumTouchState;
/* USER CODE END PTD */

/* USER CODE BEGIN Private defines */
/* Private define ------------------------------------------------------------*/

/* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断)*/

#define XPT2046_PENIRQ_GPIO_PORT   GPIOC
#define XPT2046_PENIRQ_GPIO_PIN    GPIO_PIN_12
#define XPT2046_PENIRQ_ActiveLevel 0 //触屏信号有效电平
#define XPT2046_PENIRQ_Read()      HAL_GPIO_ReadPin(XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN)

/*XPT2046 触摸屏模拟 SPI 引脚定义*/

#define XPT2046_SPI_CS_PIN         GPIO_PIN_9
#define XPT2046_SPI_CS_PORT        GPIOC

#define XPT2046_SPI_CLK_PIN        GPIO_PIN_8
#define XPT2046_SPI_CLK_PORT       GPIOC

#define XPT2046_SPI_MOSI_PIN       GPIO_PIN_10
#define XPT2046_SPI_MOSI_PORT      GPIOC

#define XPT2046_SPI_MISO_PIN       GPIO_PIN_11
#define XPT2046_SPI_MISO_PORT      GPIOC

#define XPT2046_CS_ENABLE()        digitalHi(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN)
#define XPT2046_CS_DISABLE()       digitalLo(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN)

#define XPT2046_CLK_HIGH()         digitalHi(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN)
#define XPT2046_CLK_LOW()          digitalLo(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN)

#define XPT2046_MOSI_1()           digitalHi(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN)
#define XPT2046_MOSI_0()           digitalLo(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN)

#define XPT2046_MISO()             HAL_GPIO_ReadPin(XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN)

/* XPT2046 触摸屏参数定义 */

#define XPT2046_THRESHOLD_CalDiff  2//校准触摸屏时触摸坐标的AD值相差门限

#define XPT2046_CHANNEL_X          0x90 //通道Y+的选择控制字
#define XPT2046_CHANNEL_Y          0xd0 //通道X+的选择控制字

#define FLASH_TOUCH_PARA_FLAG_VALUE 0xA5    //触摸参数写到FLASH里的标志
#define FLASH_TOUCH_PARA_FLAG_ADDR (1*1024) //触摸标志写到FLASH里的地址
#define FLASH_TOUCH_PARA_ADDR      (2*1024) //触摸参数写到FLASH里的地址


/* 信息输出 */

#define XPT2046_DEBUG_ON           1

#define XPT2046_INFO(fmt, arg...)  printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define XPT2046_ERROR(fmt, arg...) printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define XPT2046_DEBUG(fmt, arg...) do {                                                            \
                                       if(XPT2046_DEBUG_ON)                                        \
                                         printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                      } while (0)

/* 触摸状态机相关 */

#define TOUCH_PRESSED     1
#define TOUCH_NOT_PRESSED 0

#define DURIATION_TIME    2 //触摸消抖阈值
/* USER CODE END Private defines */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* 声明 XPT2046 相关的外部全局变量 */

extern volatile uint8_t ucXPT2046_TouchFlag;

extern strType_XPT2046_TouchPara strXPT2046_TouchPara[];

/* USER CODE END Variables */

/* USER CODE BEGIN Prototypes */
void XPT2046_Init(void);

uint8_t XPT2046_Touch_Calibrate(uint8_t LCD_Mode);

uint8_t XPT2046_Get_TouchedPoint(strType_XPT2046_Coordinate *displayPtr, strType_XPT2046_TouchPara *para);

void XPT2046_TouchDown(strType_XPT2046_Coordinate *touch);

void XPT2046_TouchUp(strType_XPT2046_Coordinate *touch);

void XPT2046_TouchEvenHandler(void);

void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode, uint8_t forceCal);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif //TOUCH_SCREEN_H

/************************ (C) COPYRIGHT AngelBeats *****END OF FILE****/