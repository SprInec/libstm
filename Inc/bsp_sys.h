/**
 * @file bsp_sys.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2022.06.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_SYS_H__
#define __BSP_SYS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

#ifdef MCUID_STM32F1
#include "stm32f1xx.h"
#endif
#ifdef MCUID_STM32F4
#include "stm32f4xx.h"
#endif

#define JTAG_SWD_Enable 0x00000000
#define JNTRST_Disable 0x00000001
#define SWD_Enable 0x00000010
#define JTAG_SWD_Disable 0x00000100

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0X02000000 + ((addr & 0x000FFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#ifdef MCUID_STM32F1

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

#ifdef MCUID_STM32F4
#define GPIOA_IDR_Addr (GPIOA_BASE + 0x10)
#define GPIOB_IDR_Addr (GPIOB_BASE + 0x10)
#define GPIOC_IDR_Addr (GPIOC_BASE + 0x10)
#define GPIOD_IDR_Addr (GPIOD_BASE + 0x10)
#define GPIOE_IDR_Addr (GPIOE_BASE + 0x10)
#define GPIOF_IDR_Addr (GPIOF_BASE + 0x10)
#define GPIOG_IDR_Addr (GPIOG_BASE + 0x10)

#define GPIOA_ODR_Addr (GPIOA_BASE + 0x14)
#define GPIOB_ODR_Addr (GPIOB_BASE + 0x14)
#define GPIOC_ODR_Addr (GPIOC_BASE + 0x14)
#define GPIOD_ODR_Addr (GPIOD_BASE + 0x14)
#define GPIOE_ODR_Addr (GPIOE_BASE + 0x14)
#define GPIOF_ODR_Addr (GPIOF_BASE + 0x14)
#define GPIOG_ODR_Addr (GPIOG_BASE + 0x14)
#endif

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) 
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) 
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n)
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) 
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n) 

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) 
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)  

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) 
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)  

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) 
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)  

#define __BSP_SOFTWARE_RST()    \
      {                         \
            __set_FAULTMASK(1); \
            NVIC_SystemReset(); \
      }

#ifdef __cplusplus
}
#endif
#endif
