/**
 * @file bsp_common.h
 * @author July (Email: JulyCub@163.com)
 * @brief Some Commonly used macro definition statements.
 * @version 0.1
 * @date 2022.06.12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_COMMON_H__
#define __BSP_COMMON_H__

#include "bsp_config.h"

/* -@- */
#if 1
#define __BSP_Com_ExpDef
#endif

#ifdef __BSP_Com_ExpDef

#ifdef __cplusplus
#define __I volatile /*!< Defines 'read only' permissions */
#else
#define __I volatile const /*!< Defines 'read only' permissions */
#endif
#define __O volatile  /*!< Defines 'write only' permissions */
#define __IO volatile /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define __IM volatile const /*! Defines 'read only' structure member permissions */
#define __OM volatile       /*! Defines 'write only' structure member permissions */
#define __IOM volatile      /*! Defines 'read / write' structure member permissions */

/*!< STM32x Standard Peripheral Library old types (maintained for legacy purpose) */
#if 1
    typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef const int32_t sc32; /*!< Read Only */
typedef const int16_t sc16; /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;

typedef __I int32_t vsc32; /*!< Read Only */
typedef __I int16_t vsc16; /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef const uint32_t uc32; /*!< Read Only */
typedef const uint16_t uc16; /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef __I uint32_t vuc32; /*!< Read Only */
typedef __I uint16_t vuc16; /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#endif

#endif /* __BSP_Com_ExpDef */

/* GPIO PIN */
#define GPIO_Pin_0 GPIO_PIN_0
#define GPIO_Pin_1 GPIO_PIN_1
#define GPIO_Pin_2 GPIO_PIN_2
#define GPIO_Pin_3 GPIO_PIN_3
#define GPIO_Pin_4 GPIO_PIN_4
#define GPIO_Pin_5 GPIO_PIN_5
#define GPIO_Pin_6 GPIO_PIN_6
#define GPIO_Pin_7 GPIO_PIN_7
#define GPIO_Pin_8 GPIO_PIN_8
#define GPIO_Pin_9 GPIO_PIN_9
#define GPIO_Pin_10 GPIO_PIN_10
#define GPIO_Pin_11 GPIO_PIN_11
#define GPIO_Pin_12 GPIO_PIN_12
#define GPIO_Pin_13 GPIO_PIN_13
#define GPIO_Pin_14 GPIO_PIN_14
#define GPIO_Pin_15 GPIO_PIN_15

/* GPIOMode_TypeDef */
#define GPIO_Mode_AIN GPIO_MODE_INPUT         // 模拟输入模式
#define GPIO_Mode_IN_FLOATING GPIO_MODE_INPUT // 浮空输入模式
#define GPIO_Mode_IPD GPIO_MODE_INPUT         // 上拉输入模式
#define GPIO_Mode_IPU GPIO_MODE_INPUT         // 下拉输入模式
#define GPIO_Mode_Out_OD GPIO_MODE_OUTPUT_OD  // 开漏输出模式
#define GPIO_Mode_OUT GPIO_MODE_OUTPUT_PP     // 推挽输出模式
#define GPIO_Mode_Out_PP GPIO_MODE_OUTPUT_PP  // 推挽输出模式
#define GPIO_Mode_AF_OD GPIO_MODE_AF_OD       // 复用开漏输出模式
#define GPIO_Mode_AF_PP GPIO_MODE_AF_PP       // 复用推挽输出模式

#define GPIO_PuPd_UP GPIO_PULLUP
#define GPIO_PuPd_DN GPIO_PULLDOWN

/* GPIOSpeed_TypeDef */
#define GPIO_Speed_10MHz GPIO_SPEED_FREQ_MEDIUM     // 中速
#define GPIO_Speed_2MHz GPIO_SPEED_FREQ_LOW         // 低速
#define GPIO_Speed_50MHz GPIO_SPEED_FREQ_MEDIUM     // 高速
#define GPIO_Speed_100MHz GPIO_SPEED_FREQ_VERY_HIGH // 超高速

/* FSMC_Access_Mode Typedef */
#define FSMC_AccessMode_A FSMC_ACCESS_MODE_A
#define FSMC_AccessMode_B FSMC_ACCESS_MODE_B
#define FSMC_AccessMode_C FSMC_ACCESS_MODE_C
#define FSMC_AccessMode_D FSMC_ACCESS_MODE_D

#define GPIO_Init(n1, n2) HAL_GPIO_Init(n1, n2)

#define GPIO_SetBits(n1, n2) HAL_GPIO_WritePin(n1, n2, GPIO_PIN_SET)
#define GPIO_ResetBits(n1, n2) HAL_GPIO_WritePin(n1, n2, GPIO_PIN_RESET)

#define __BSP_RCC_GPIO_ENABLE(__GPIO__) __HAL_RCC_##__GPIO__##_CLK_ENABLE()
#define __BSP_RCC_GPIO_DISABLE(__GPIO__) __HAL_RCC_##__GPIO__##_CLK_DISABLE()

#endif /* __BSP_COMMON_H__ */
