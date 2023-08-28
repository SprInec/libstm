/**
 * @file bsp_quadspi.h
 * @author July (Email: JulyCub@163.com)
 * @brief BSP QUADSPI Driver
 * @version 0.1
 * @date 2023.07.16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_QUADSPI_H__
#define __BSP_QUADSPI_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

void QSPI_Send_CMD( u32 instruction, 
                    u32 address, 
                    u32 dummyCycles, 
                    u32 instructionMode, 
                    u32 addressMode, 
                    u32 addressSize, 
                    u32 dataMode);

u8 QSPI_Receive(u8 *buf, u32 datalen);
u8 QSPI_Transmit(u8 *buf, u32 datalen);

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_QUADSPI_H__ */
