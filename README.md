# RTOS
My FreeRTOS to drive stm32.Including ADC, DMA, FLASH, LCD, SPI, USART, TIM. LCD driven by ILI9341.

### Environment
1. CubeMX
2. IDE
    1. Use Visual Studio.
       When using VS, you need a tool named VisualGDB.
    2. Use CLion.
       Clion has supported embeded projects now.
    3. Use other editors.
       Any editor that configure with cmake, or you can choose KEIL :) .

### Contents
```
.
├── Core
│   ├── Inc
│   │   ├── adc.h
│   │   ├── crc.h
│   │   ├── dma.h
│   │   ├── font.h
│   │   ├── FreeRTOSConfig.h
│   │   ├── gpio.h
│   │   ├── lcd.h
│   │   ├── main.h
│   │   ├── spi.h
│   │   ├── stm32f1xx_hal_conf.h
│   │   ├── stm32f1xx_it.h
│   │   ├── tim.h
│   │   ├── touch_screen.h
│   │   └── usart.h
│   └── Src
│       ├── adc.c
│       ├── crc.c
│       ├── dma.c
│       ├── font.c
│       ├── freertos.c
│       ├── gpio.c
│       ├── lcd.c
│       ├── main.c
│       ├── spi.c
│       ├── stm32f1xx_hal_msp.c
│       ├── stm32f1xx_hal_timebase_tim.c
│       ├── stm32f1xx_it.c
│       ├── syscalls.c
│       ├── system_stm32f1xx.c
│       ├── tim.c
│       ├── touch_screen.c
│       └── usart.c
├── Drivers
│   ├── CMSIS
│   │   ├── Device
│   │   │   └── ST
│   │   │       └── STM32F1xx
│   │   │           ├── Include
│   │   │           │   ├── stm32f103xe.h
│   │   │           │   ├── stm32f1xx.h
│   │   │           │   └── system_stm32f1xx.h
│   │   │           └── Source
│   │   │               └── Templates
│   │   └── Include
│   │       ├── cmsis_armcc.h
│   │       ├── cmsis_armclang.h
│   │       ├── cmsis_compiler.h
│   │       ├── cmsis_gcc.h
│   │       ├── cmsis_iccarm.h
│   │       ├── cmsis_version.h
│   │       ├── core_armv8mbl.h
│   │       ├── core_armv8mml.h
│   │       ├── core_cm0.h
│   │       ├── core_cm0plus.h
│   │       ├── core_cm1.h
│   │       ├── core_cm23.h
│   │       ├── core_cm33.h
│   │       ├── core_cm3.h
│   │       ├── core_cm4.h
│   │       ├── core_cm7.h
│   │       ├── core_sc000.h
│   │       ├── core_sc300.h
│   │       ├── mpu_armv7.h
│   │       ├── mpu_armv8.h
│   │       └── tz_context.h
│   └── STM32F1xx_HAL_Driver
│       ├── Inc
│       │   ├── Legacy
│       │   │   └── stm32_hal_legacy.h
│       │   ├── stm32f1xx_hal_adc_ex.h
│       │   ├── stm32f1xx_hal_adc.h
│       │   ├── stm32f1xx_hal_cortex.h
│       │   ├── stm32f1xx_hal_crc.h
│       │   ├── stm32f1xx_hal_def.h
│       │   ├── stm32f1xx_hal_dma_ex.h
│       │   ├── stm32f1xx_hal_dma.h
│       │   ├── stm32f1xx_hal_exti.h
│       │   ├── stm32f1xx_hal_flash_ex.h
│       │   ├── stm32f1xx_hal_flash.h
│       │   ├── stm32f1xx_hal_gpio_ex.h
│       │   ├── stm32f1xx_hal_gpio.h
│       │   ├── stm32f1xx_hal.h
│       │   ├── stm32f1xx_hal_pwr.h
│       │   ├── stm32f1xx_hal_rcc_ex.h
│       │   ├── stm32f1xx_hal_rcc.h
│       │   ├── stm32f1xx_hal_spi.h
│       │   ├── stm32f1xx_hal_tim_ex.h
│       │   ├── stm32f1xx_hal_tim.h
│       │   └── stm32f1xx_hal_uart.h
│       └── Src
│           ├── stm32f1xx_hal_adc.c
│           ├── stm32f1xx_hal_adc_ex.c
│           ├── stm32f1xx_hal.c
│           ├── stm32f1xx_hal_cortex.c
│           ├── stm32f1xx_hal_crc.c
│           ├── stm32f1xx_hal_dma.c
│           ├── stm32f1xx_hal_exti.c
│           ├── stm32f1xx_hal_flash.c
│           ├── stm32f1xx_hal_flash_ex.c
│           ├── stm32f1xx_hal_gpio.c
│           ├── stm32f1xx_hal_gpio_ex.c
│           ├── stm32f1xx_hal_pwr.c
│           ├── stm32f1xx_hal_rcc.c
│           ├── stm32f1xx_hal_rcc_ex.c
│           ├── stm32f1xx_hal_spi.c
│           ├── stm32f1xx_hal_tim.c
│           ├── stm32f1xx_hal_tim_ex.c
│           └── stm32f1xx_hal_uart.c
├── FATFS
│   ├── App
│   │   ├── fatfs.c
│   │   └── fatfs.h
│   └── Target
│       ├── ffconf.h
│       ├── user_diskio.c
│       └── user_diskio.h
├── Middlewares
│   └── Third_Party
│       ├── FatFs
│       │   └── src
│       │       ├── diskio.c
│       │       ├── diskio.h
│       │       ├── ff.c
│       │       ├── ff_gen_drv.c
│       │       ├── ff_gen_drv.h
│       │       ├── ff.h
│       │       ├── integer.h
│       │       └── option
│       │           ├── ccsbcs.c
│       │           └── syscall.c
│       └── FreeRTOS
│           └── Source
│               ├── CMSIS_RTOS
│               │   ├── cmsis_os.c
│               │   └── cmsis_os.h
│               ├── croutine.c
│               ├── event_groups.c
│               ├── include
│               │   ├── croutine.h
│               │   ├── deprecated_definitions.h
│               │   ├── event_groups.h
│               │   ├── FreeRTOS.h
│               │   ├── list.h
│               │   ├── message_buffer.h
│               │   ├── mpu_prototypes.h
│               │   ├── mpu_wrappers.h
│               │   ├── portable.h
│               │   ├── projdefs.h
│               │   ├── queue.h
│               │   ├── semphr.h
│               │   ├── stack_macros.h
│               │   ├── StackMacros.h
│               │   ├── stream_buffer.h
│               │   ├── task.h
│               │   └── timers.h
│               ├── list.c
│               ├── portable
│               │   ├── GCC
│               │   │   └── ARM_CM3
│               │   │       ├── port.c
│               │   │       └── portmacro.h
│               │   └── MemMang
│               │       └── heap_3.c
│               ├── queue.c
│               ├── stream_buffer.c
│               ├── tasks.c
│               └── timers.c
├── startup
│   └── startup_stm32f103xe.s
├── Project4.ioc
├── README.md
├── LICENSE
├── .gitignore
└── STM32F103RCTx_FLASH.ld
```

### To-do List

- [ ] UI
- [x] Migarate to H743
- [ ] emWin


### Update V1.0

- 我把底层迁移到了CubeMX的CMSIS_RTOS的V1版本，因为这个版本对
于F1系列来说是高效又稳定的。CMSIS_RTOS的V2版本还不够完善，没有做到对于FreeRTOS和
STemWin的完全适配。
- 另外我给这个demo增添了FatFs文件系统，更改了对于触摸屏的校准支持。
- 这个项目目前已经成功迁移到了H743上，之后的emWin与UI将在H743上继续开发。
