/**
 * @file bsp_lmk61e07.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.07.11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_LMK61E07_H__
#define __BSP_LMK61E07_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

#define LMK61E07_SLAVEADR 0x59
#define LMK61E07_WR ((LMK61E07_SLAVEADR << 1) | 0x00)
#define LMK61E07_RD ((LMK61E07_SLAVEADR << 1) | 0x01)

/* REGISTER DEFINE */
#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R8 0x08
#define R9 0x09
#define R10 0x10
#define R16 0x16
#define R17 0x17
#define R21 0x21
#define R22 0x22
#define R23 0x23
#define R24 0x24
#define R25 0x25
#define R26 0x26
#define R27 0x27
#define R28 0x28
#define R29 0x29
#define R30 0x30
#define R31 0x31
#define R32 0x32
#define R33 0x33
#define R34 0x34
#define R35 0x35
#define R36 0x36
#define R37 0x37
#define R38 0x38
#define R39 0x39
#define R42 0x42
#define R47 0x47
#define R48 0x48
#define R49 0x49
#define R50 0x50
#define R51 0x51
#define R52 0x52
#define R53 0x53
#define R56 0x56
#define R66 0x66
#define R72 0x72

#define VNDRID_BY1 R0
#define VNDRID_BY0 R1
#define PRODID R2
#define REVID R3
#define SLAVEADR R8
#define EEREV R9
#define DEV_CTL R10
#define XO_CAPCTRL_BY1 R16
#define XO_CAPCTRL_BY0 R17
#define DIFFCTL R21
#define OUTDIV_BY1 R22
#define OUTDIV_BY0 R23
#define RDIVCMOSCTL R24
#define PLL_NDIV_BY1 R25
#define PLL_NDIV_BY0 R25
#define PLL_FRACNUM_BY2 R27
#define PLL_FRACNUM_BY1 R28
#define PLL_FRACNUM_BY0 R29
#define PLL_FRACDEN_BY2 R30
#define PLL_FRACDEN_BY1 R31
#define PLL_FRACDEN_BY0 R32
#define PLL_MASHCTRL R33
#define PLL_CTRL0 R34
#define PLL_CTRL1 R35
#define PLL_LF_R2 R36
#define PLL_LF_C1 R37
#define PLL_LF_R3 R38
#define PLL_LF_C3 R39
#define PLL_CALCTRL R42
#define NVMSCRC R47
#define NVMCNT R48
#define NVMCTL R49
#define NVMLCRC R50
#define MEMADR R51
#define NVMDAT R52
#define RAMDAT R53
#define NVMUNLK R56
#define INT_LIVE R66
#define SWRST R72

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_LMK61E07_H__ */
