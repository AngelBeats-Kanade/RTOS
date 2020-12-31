# RTOS
My FreeRTOS to drive stm32.Including ADC, DMA, FLASH, LCD, SPI, USART, TIM. LCD driven by ILI9341, can display Chinese. Not including global interrupt and animation functions. Now I`m thinking about migrating it to stm32h743.

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
├─ .gitignore
├─ LICENSE
├─ Project4.ioc
├─ README.md
├─ STM32F103RCTx_FLASH.ld
│
├─ Core //具体实现文件
│  ├─ Inc
│  │  ├─adc.h
│  │  ├─dma.h
│  │  ├─font.h
│  │  ├─FreeRTOSConfig.h
│  │  ├─gpio.h
│  │  ├─lcd.h
│  │  ├─main.h
│  │  ├─spi.h
│  │  ├─stm32f1xx_hal_conf.h
│  │  ├─stm32f1xx_it.h
│  │  ├─tim.h
│  │  └─usart.h
│  │
│  └─Src
│    ├─adc.c
│    ├─dma.c
│    ├─font.c
│    ├─freertos.c
│    ├─gpio.c
│    ├─lcd.c
│    ├─main.c
│    ├─spi.c
│    ├─stm32f1xx_hal_msp.c
│    ├─stm32f1xx_hal_timebase_tim.c
│    ├─stm32f1xx_it.c
│    ├─syscalls.c
│    ├─system_stm32f1xx.c
│    ├─tim.c
│    └─usart.c
│
├─Drivers //驱动文件，HAL库
│  ├─CMSIS
│  │  ├─Device
│  │  │  └─ST
│  │  │    └─STM32F1xx
│  │  │      ├─Include
│  │  │      │ ├─stm32f103xe.h
│  │  │      │ ├─stm32f1xx.h
│  │  │      │ └─system_stm32f1xx.h
│  │  │      │
│  │  │      └─Source
│  │  │        └─Templates
│  │  └─Include
│  │    ├─cmsis_armcc.h
│  │    ├─cmsis_armclang.h
│  │    ├─cmsis_compiler.h
│  │    ├─cmsis_gcc.h
│  │    ├─cmsis_iccarm.h
│  │    ├─cmsis_version.h
│  │    ├─core_armv8mbl.h
│  │    ├─core_armv8mml.h
│  │    ├─core_cm0.h
│  │    ├─core_cm0plus.h
│  │    ├─core_cm1.h
│  │    ├─core_cm23.h
│  │    ├─core_cm3.h
│  │    ├─core_cm33.h
│  │    ├─core_cm4.h
│  │    ├─core_cm7.h
│  │    ├─core_sc000.h
│  │    ├─core_sc300.h
│  │    ├─mpu_armv7.h
│  │    ├─mpu_armv8.h
│  │    └─tz_context.h
│  │
│  └─STM32F1xx_HAL_Driver
│      ├─Inc
│      │ ├─stm32f1xx_hal.h
│      │ ├─stm32f1xx_hal_adc.h
│      │ ├─stm32f1xx_hal_adc_ex.h
│      │ ├─stm32f1xx_hal_cortex.h
│      │ ├─stm32f1xx_hal_def.h
│      │ ├─stm32f1xx_hal_dma.h
│      │ ├─stm32f1xx_hal_dma_ex.h
│      │ ├─stm32f1xx_hal_exti.h
│      │ ├─stm32f1xx_hal_flash.h
│      │ ├─stm32f1xx_hal_flash_ex.h
│      │ ├─stm32f1xx_hal_gpio.h
│      │ ├─stm32f1xx_hal_gpio_ex.h
│      │ ├─stm32f1xx_hal_pwr.h
│      │ ├─stm32f1xx_hal_rcc.h
│      │ ├─stm32f1xx_hal_rcc_ex.h
│      │ ├─stm32f1xx_hal_spi.h
│      │ ├─stm32f1xx_hal_tim.h
│      │ ├─stm32f1xx_hal_tim_ex.h
│      │ ├─stm32f1xx_hal_uart.h
│      │ │
│      │ └─Legacy
│      │   └─stm32_hal_legacy.h
│      │
│      └─Src
│        ├─stm32f1xx_hal.c
│        ├─stm32f1xx_hal_adc.c
│        ├─stm32f1xx_hal_adc_ex.c
│        ├─stm32f1xx_hal_cortex.c
│        ├─stm32f1xx_hal_dma.c
│        ├─stm32f1xx_hal_exti.c
│        ├─stm32f1xx_hal_flash.c
│        ├─stm32f1xx_hal_flash_ex.c
│        ├─stm32f1xx_hal_gpio.c
│        ├─stm32f1xx_hal_gpio_ex.c
│        ├─stm32f1xx_hal_pwr.c
│        ├─stm32f1xx_hal_rcc.c
│        ├─stm32f1xx_hal_rcc_ex.c
│        ├─stm32f1xx_hal_spi.c
│        ├─stm32f1xx_hal_tim.c
│        ├─stm32f1xx_hal_tim_ex.c
│        └─stm32f1xx_hal_uart.c
│
├─Middlewares //FreeRTOS库文件
│ └─Third_Party
│    └─FreeRTOS
│      └─Source
│        ├─croutine.c
│        ├─event_groups.c
│        ├─list.c
│        ├─queue.c
│        ├─stream_buffer.c
│        ├─tasks.c
│        ├─timers.c
│        │
│        ├─CMSIS_RTOS_V2
│        │ ├─cmsis_os.h
│        │ ├─cmsis_os2.c
│        │ ├─cmsis_os2.h
│        │
│        ├─include
│        │ ├─croutine.h
│        │ ├─deprecated_definitions.h
│        │ ├─event_groups.h
│        │ ├─FreeRTOS.h
│        │ ├─list.h
│        │ ├─message_buffer.h
│        │ ├─mpu_prototypes.h
│        │ ├─mpu_wrappers.h
│        │ ├─portable.h
│        │ ├─projdefs.h
│        │ ├─queue.h
│        │ ├─semphr.h
│        │ ├─StackMacros.h
│        │ ├─stack_macros.h
│        │ ├─stream_buffer.h
│        │ ├─task.h
│        │ └─timers.h
│        │
│        └─portable
│          ├─GCC
│          │ └─ARM_CM3
│          │   ├─port.c
│          │   └─portmacro.h
│          │
│          └─MemMang
│            └─heap_3.c
└─startup //启动文件
  └─startup_stm32f103xe.s
```

### To-do List

- [ ] UI
- [ ] Migarate to H743
- [ ] emWin
