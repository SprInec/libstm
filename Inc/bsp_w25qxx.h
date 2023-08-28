/**
 * @file bsp_w25qxx.h
 * @author DevEBox
 * @brief w25qxx qpi driver 
 * @version 0.1
 * @date 2023.07.16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_W25QXX_H__
#define __BSP_W25QXX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

/**
 * @brief W25X系列/Q系列芯片列表
 *      W25Q80  ID  0XEF13
 *      W25Q16  ID  0XEF14
 *      W25Q32  ID  0XEF15
 *      W25Q64  ID  0XEF16
 *      W25Q128 ID  0XEF17
 *      W25Q256 ID  0XEF18
 */
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

/* 定义W25QXX芯片型号 */
extern u16 W25QXX_TYPE;   

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

#define W25X_SetReadParam		0xC0 
#define W25X_EnterQPIMode       0x38
#define W25X_ExitQPIMode        0xFF

void W25QXX_Init(void);					//初始化W25QXX
void W25QXX_Qspi_Enable(void);			//使能QSPI模式
void W25QXX_Qspi_Disable(void);			//关闭QSPI模式
u16  W25QXX_ReadID(void);  	    		//读取FLASH ID
u8 W25QXX_ReadSR(u8 regno);             //读取状态寄存器 
void W25QXX_Write_SR(u8 regno,u8 sr);   //写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写flash,不校验
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25QXX_Wait_Busy(void);           	//等待空闲

#ifdef __cplusplus
}
#endif
#endif
