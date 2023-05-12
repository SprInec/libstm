/**
 * @file bsp_i2c.h
 * @author CT117E-M4嵌入式竞赛
 * @brief 软件I2C
 * @version 0.1
 * @date 2020.03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "bsp_config.h"

void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
unsigned char I2CWaitNoAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void I2CInit(void);

#endif
