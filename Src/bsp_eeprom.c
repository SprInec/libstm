/**
 * @file bsp_eeprom.c
 * @author July (Email: JulyCub@163.com)
 * @brief EEPROM implementation
 * @version 0.1
 * @date 2023.03.21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_eeprom.h"
#include "bsp_i2c.h"

/**
 * @brief eeprom 初始化
 */
void bsp_eeprom_Init(void)
{
	I2CInit();
}

/**
 * @brief eeprom 按字节写入
 * 
 * @param data 写入数据
 * @param data_addr 写入地址
 * @return uint8_t 
 */
uint8_t bsp_eeprom_writeByte(uint8_t data, uint8_t data_addr)
{
	I2CStart();
	/* 设备地址及操作位 */
	I2CSendByte(EEPROM_ADDR | 0x00);
	I2CWaitAck();
	/* 数据字地址 */
	I2CSendByte(data_addr);
	I2CWaitAck();
	/* 数据 */
	I2CSendByte(data);
	I2CWaitAck();
	I2CStop();
	return SUCCESS;
}

/**
 * @brief eeprom 按页写入
 * 
 * @param data 写入数据
 * @param data_addr 写入地址
 * @param num 写入字节数
 * @return uint8_t
 */
uint8_t bsp_eeprom_writePage(uint8_t * data, uint8_t data_addr, uint8_t num)
{
	I2CStart();
	/* 设备地址及操作位 */
	I2CSendByte(EEPROM_ADDR | 0x00);
	I2CWaitAck();
	/* 数据段地址 */
	I2CSendByte(data_addr);
	I2CWaitAck();
	/* 数据 */
	for (uint8_t i = 0; i < num; i++)
	{
		I2CSendByte(*(data + i));
		I2CWaitAck();
	}
	I2CStop();
	return SUCCESS;
}

/**
 * @brief eeprom 按字节读取
 * 
 * @param data 读取数据存放地址
 * @param data_addr 读取地址
 * @return uint8_t 
 */
uint8_t bsp_eeprom_readByte(uint8_t * data, uint8_t data_addr)
{
	I2CStart();
	I2CSendByte(EEPROM_ADDR | 0x00);
	I2CWaitAck();
	I2CSendByte(data_addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(EEPROM_ADDR | 0x01);
	I2CWaitAck();
	*data = I2CReceiveByte();
	I2CWaitNoAck();
	I2CStop();
	return SUCCESS;
}

/**
 * @brief eeprom 按字节连续读取
 * 
 * @param data 读取数据存放地址
 * @param data_addr 读取地址
 * @param num 读取字节数
 * @return uint8_t 
 */
uint8_t bsp_eeprom_readSeqByte(uint8_t * data, uint8_t data_addr, uint8_t num)
{
	I2CStart();
	I2CSendByte(EEPROM_ADDR | 0x00);
	I2CWaitAck();
	I2CSendByte(data_addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(EEPROM_ADDR | 0x01);
	I2CWaitAck();
	for (uint8_t i = 0; i < num; i++)
	{
		*(data + i) = I2CReceiveByte();
		if (i == num - 1)
			I2CWaitNoAck();
		else
			I2CSendAck();
	}
	I2CStop();
	return SUCCESS;
}


