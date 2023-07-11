/**
 * @file bsp_pe4302.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.07.11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _PE4302_H_
#define _PE4302_H_

#include "bsp_sys.h"
#include "bsp_delay.h"

/***********************
 * MULTI-PE4302 CONFIG *
 ***********************/
#if 1
#define PE4302_USING_MULT
#endif

/* DB values */
#define DB_0 ((u8)0X00)
#define DB_0_5 ((u8)0X01)
#define DB_1 ((u8)0X02)
#define DB_2 ((u8)0X04)
#define DB_4 ((u8)0X08)
#define DB_8 ((u8)0X10)
#define DB_16 ((u8)0X20)
#define DB_31_5 ((u8)0X3F)

#ifndef PE4302_USING_MULT
#define PE_LE PCout(2)
#define PE_CLK PCout(1)
#define PE_DAT PCout(0)

#define PE_LE_EN PE_LE = 1
#define PE_LE_0_DIS PE_LE = 0
#define PE_CLK_0 PE_CLK = 0
#define PE_DAT_0 PE_DAT = 0
#else
#define PE_LE_0 PCout(2)
#define PE_LE_1 PCout(3)
#define PE_CLK PCout(1)
#define PE_DAT PCout(0)

#define PE_LE_0_EN (PE_LE_0 = 1)
#define PE_LE_1_EN (PE_LE_1 = 1)

#define PE_LE_0_DIS (PE_LE_0 = 0)
#define PE_LE_1_DIS (PE_LE_1 = 0)
#endif /* !PE4302_USING_MULT */

#define PE_CLK_0 (PE_CLK = 0)
#define PE_CLK_1 (PE_CLK = 1)

#define PE_DAT_0 (PE_DAT = 0)
#define PE_DAT_1 (PE_DAT = 1)

#ifndef PE4302_USING_MULT
void BSP_PE4302_Init(void);
void BSP_PE4302_Set(unsigned char db);
#else
void BSP_PE4302_Init(void);
void BSP_PE4302_0_Set(unsigned char db);
void BSP_PE4302_1_Set(unsigned char db);
#endif /* !PE4302_USING_MULT */
#endif
