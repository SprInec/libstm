/**
 * @file bsp_lmk61e07.c
 * @author July (Email: JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2023.07.11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_lmk61e07.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"

/**
 * @brief LMK61E06 initlization
 */
void BSP_LMK61E07_Init(void)
{
    I2CInit();
}

/**
 * @brief LMK61E07 write register
 * 
 * @param address register address
 * @param data data
 */
void BSP_LMK61E07_WriteREG(uint8_t address, uint8_t data)
{
    I2CStart();
    I2CSendByte(LMK61E07_WR);
    I2CWaitAck();
    I2CSendByte(address);
    I2CWaitAck();

    I2CSendByte(data);
    I2CWaitAck();

    I2CStop();
}

/**
 * @brief LMK61E07 read register
 * 
 * @param address register address
 * @return uint8_t read data
 */
uint8_t BSP_LMK61E07_ReadREG(uint8_t address)
{
    uint8_t data;

    I2CStart();
    I2CSendByte(LMK61E07_WR);
    I2CWaitAck();
    I2CSendByte(address);
    I2CWaitAck();

    I2CStart();
    I2CSendByte(LMK61E07_RD);
    I2CWaitAck();

    data = I2CReceiveByte();
    I2CSendAck();
    I2CStop();

    return data;
}

/**
 * @brief save the value of register to sram
 */
void BSP_LMK61E07_WriteREG2SRAM(void)
{
    BSP_LMK61E07_WriteREG(R49, (0x10 | (1 << 6)));
}

/**
 * @brief save the value of register to eeprom
 */
void BSP_LMK61E07_WriteEEPROM(void)
{
    BSP_LMK61E07_WriteREG(R56, 0xBE);
    BSP_LMK61E07_WriteREG(R49, (0x10 | (1 << 0)));
    BSP_LMK61E07_WriteREG(R56, 0x00);
}

// TODO: Complete the code
void BSP_LMK61E07_SetFreqency(void)
{

}