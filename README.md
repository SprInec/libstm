# README

## 简介

该项目是基于 STM32 HAL 库的 BSP（Board Support Package），整合了本人搜集和自编的相关 BSP 代码，并包含一个 `bsp_config.h` 头文件，以便统一配置与管理所有资源。此项目旨在加速和提高 STM32 应用的开发效率。

- `bsp_config.h` 支持 *Configuration Wizard* 可视化配置
- 支持 CMSIS DSP 库

## 文件结构

- `README.md`: 本文件，提供项目说明
- `Inc/`: BSP 头文件
- `Src/`: BSP 源代码
- `scripts/`: 一些 STM 开发中可能用到的脚本文件
- `bsp_user.c`:  用户源文件
- `bsp_user.h`:  用户头文件
- `bsp_config.h`:  libstm 配置文件

## 包含组件

|                  类别 | 组件           |
| --------------------: | -------------- |
|      模数转换器 (ADC) | AD7606         |
| 数字-模拟转换器 (DAC) | DAC8563        |
|    数字信号处理 (DSP) | CMSIS DSP FFT  |
|                       | FILTER         |
|            数控衰减器 | PE4302         |
|            数字合成器 | AD9833         |
|                       | AD9851         |
|                       | AD9854         |
|                       | AD9959         |
|            频率合成器 | ADF4002        |
|                       | ADF4351        |
|                传感器 | BMP280         |
|                       | DHT11          |
|                       | DHT20          |
|                       | DS18B20        |
|                       | GP2Y           |
|                       | VMS WINDSENSOR |
|                       | OV2640         |
|                       | S12SD          |
|                  存储 | EEPROM         |
|                       | W25Qxx         |
|              显示模块 | LCD1602        |
|                       | LCD TFT        |
|                       | OLED           |
|                       | TJC HMI        |
|              通信接口 | I2C            |
|                       | RS485          |
|                       | USART          |
|                       | QUADSPI        |
|                       | SCCB           |

## 使用方法

1. 克隆或下载该项目源代码。

    ```bash
    git clone https://github.com/SprInec/libstm.git
    ```

2. 直接将 libstm 整个文件夹复制到你的项目根目录。

3. 根据你的实际应用，修改 libstm 提供的 `bsp_config.h` 配置文件。

4. 使用 libstm 进行开发。

## 支持的 MCU

-   STM32F1 系列
-   STM32F4 系列
-   STM32H7 系列
-   STM32G4 系列

## 支持的 RTOS

- FreeRTOS

- RT-Thread

## 已适配的开发板

-   野火F103指南者开发板
-   DevEBoxF103RCxx 开发板
-   DevEBoxF407ZGxx 开发板
-   DevEBoxH743xx 开发板
-   NUCLEO-H7A3ZI-Q 开发板

## 贡献

如果您发现此库中存在问题或者有任何改进建议，请随时提交 Issue或者 Pull Request，让我们共同完善 libstm。

## 联系方式

如有任何问题，请联系项目作者：JulyCub@163.com。
