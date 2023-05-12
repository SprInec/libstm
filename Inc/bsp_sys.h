/*
 ******************************************************************************
 * @file    :bsp_sys.h
 * @brief   :The board support package for sys.
 * @version :0.1.0
 * @author  :正点原子
 * @date    :2022.06.28
 ******************************************************************************
 * @attention
 *
 *    位带操作,实现51类似的GPIO控制功能
 *    具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
 *
 ******************************************************************************
 */

#ifndef __BSP_SYS_H__
#define __BSP_SYS_H__

#include "bsp_config.h"

#ifdef __BSP_STM32F1_ENABLED
#include "stm32f1xx.h"
#endif
#ifdef __BSP_STM32F4_ENABLED
#include "stm32f4xx.h"
#endif
#ifdef __BSP_STM32G4_ENABLED
#include "stm32g4xx.h"
#endif
#ifdef __BSP_STM32H7_ENABLED
#include "stm32h7xx.h"
#endif

/**********************************************************
                    JTAG模式设置定义
**********************************************************/

#define JTAG_SWD_Enable 0x00000000  // 复位值
#define JNTRST_Disable 0x00000001   // JNTRST引脚释放
#define SWD_Enable 0x00000010       // JTAG关闭，SWD开启
#define JTAG_SWD_Disable 0x00000100 // JTAG和SWD都关闭

/**********************************************************
                    位带操作相关宏定义
              参考《CM3权威指南》第87 ~ 92页
**********************************************************/

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0X02000000 + ((addr & 0x000FFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

/**********************************************************
                      GPIO地址映射
              基地址加上寄存器偏移地址组成
**********************************************************/
#ifdef __BSP_STM32F1_ENABLED

#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 12) // 0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 8) // 0x40011E08

#endif

#ifdef __BSP_STM32F4_ENABLED

#define GPIOA_IDR_Addr (GPIOA_BASE + 10)
#define GPIOB_IDR_Addr (GPIOB_BASE + 10)
#define GPIOC_IDR_Addr (GPIOC_BASE + 10)
#define GPIOD_IDR_Addr (GPIOD_BASE + 10)
#define GPIOE_IDR_Addr (GPIOE_BASE + 10)
#define GPIOF_IDR_Addr (GPIOF_BASE + 10)
#define GPIOG_IDR_Addr (GPIOG_BASE + 10)

#define GPIOA_ODR_Addr (GPIOA_BASE + 14)
#define GPIOB_ODR_Addr (GPIOB_BASE + 14)
#define GPIOC_ODR_Addr (GPIOC_BASE + 14)
#define GPIOD_ODR_Addr (GPIOD_BASE + 14)
#define GPIOE_ODR_Addr (GPIOE_BASE + 14)
#define GPIOF_ODR_Addr (GPIOF_BASE + 14)
#define GPIOG_ODR_Addr (GPIOG_BASE + 14)

#endif

#ifdef __BSP_STM32H7_ENABLED

#define GPIOA_IDR_Addr (GPIOA_BASE + 10)
#define GPIOB_IDR_Addr (GPIOB_BASE + 10)
#define GPIOC_IDR_Addr (GPIOC_BASE + 10)
#define GPIOD_IDR_Addr (GPIOD_BASE + 10)
#define GPIOE_IDR_Addr (GPIOE_BASE + 10)
#define GPIOF_IDR_Addr (GPIOF_BASE + 10)
#define GPIOG_IDR_Addr (GPIOG_BASE + 10)

#define GPIOA_ODR_Addr (GPIOA_BASE + 14)
#define GPIOB_ODR_Addr (GPIOB_BASE + 14)
#define GPIOC_ODR_Addr (GPIOC_BASE + 14)
#define GPIOD_ODR_Addr (GPIOD_BASE + 14)
#define GPIOE_ODR_Addr (GPIOE_BASE + 14)
#define GPIOF_ODR_Addr (GPIOF_BASE + 14)
#define GPIOG_ODR_Addr (GPIOG_BASE + 14)

#endif

/**********************************************************
             实现单一IO操作，类似于51的IO操作
                   n值要小于IO具体数目
**********************************************************/

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  // 输入

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)  // 输入

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)  // 输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)  // 输入

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)  // 输入

#endif
