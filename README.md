# README
## 简介

该项目是一个基于STM32 HAL的BSP（Board Support Package），旨在提供一个通用的硬件抽象层，使得开发者可以更加方便地开发STM32的应用。该BSP支持多种STM32系列芯片，并提供了一些常用的外设模块驱动，包括AD9959、AD9833、ADF4351、DAC8563、DS18B20、OLED、LCD、I2C、SCCB、USART、EEPROM、LED等，以及部分软件算法，如FIR滤波，数字编解码、DELAY等。

## 文件结构

- `README.md`: 本文件，提供项目说明
- `Inc/`: BSP头文件
- `Src/`: BSP源代码
- `bsp_user.c/`: 用户源文件
- `bsp_user.h/`: 用户头文件

## 使用方法

1. 克隆或下载该项目源代码。

    ```bash
    git clone <https://github.com/JulyCub/stm32-bsp-library.git>
    ```

2. 在你的项目中添加该BSP的源代码。

3. 根据你的实际应用，修改BSP中提供的`Inc/bsp_config.h`配置文件。

4. 在你的应用程序中使用BSP提供的API进行开发。

## 支持的芯片型号

- STM32F1系列
- STM32F4系列
- STM32G4系列
- STM32H7系列

## 贡献

如果您发现此库中存在问题或者有任何改进建议，请随时提交Issue或者Pull Request，让我们共同完善此BSP库。

## 联系方式

如有任何问题，请联系项目作者：JulyCub@163.com。