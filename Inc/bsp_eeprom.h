/**
 * @file bsp_eeprom.h
 * @author July (Email: JulyCub@163.com)
 * @brief EEPROM implementation
 * @version 0.1
 * @date 2023.03.21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __BSP_EEPROM
#define __BSP_EEPROM

#include "bsp_config.h"

#ifndef __BSP_I2C_ENABLED
#define __BSP_I2C_ENABLED
#endif
#ifndef __BSP_COMMON_ENABLED
#define __BSP_COMMON_ENABLED
#endif

#define EEPROM_ADDR 0xA0	

void bsp_eeprom_Init(void);
uint8_t bsp_eeprom_writeByte(uint8_t data, uint8_t data_addr);
uint8_t bsp_eeprom_writePage(uint8_t * data, uint8_t data_addr, uint8_t num);
uint8_t bsp_eeprom_readByte(uint8_t * data, uint8_t data_addr);
uint8_t bsp_eeprom_readSeqByte(uint8_t * data, uint8_t data_addr, uint8_t num);

#endif