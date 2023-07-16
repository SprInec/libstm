/**
 * @file bsp_quadspi.c
 * @author July (Email: JulyCub@163.com)
 * @brief BSP QUADSPI Driver 
 * @version 0.1
 * @date 2023.07.16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "quadspi.h"
#include "bsp_quadspi.h"

/**
 * @brief QSPI-发送指令
 *
 * @param instruction 待发送指令
 * @param address 目的地址
 * @param dummyCycles 空指令周期数
 * @param instructionMode 指令模式
 *              --
 *              | QSPI_INSTRUCTION_NONE
 *              | QSPI_INSTRUCTION_1_LINE
 *              | QSPI_INSTRUCTION_2_LINE
 *              | QSPI_INSTRUCTION_4_LINE
 *
 * @param addressMode 地址模式
 *              --
 *              | QSPI_ADDRESS_NONE
 *              | QSPI_ADDRESS_1_LINE
 *              | QSPI_ADDRESS_2_LINE
 *              | QSPI_ADDRESS_4_LINE
 *
 * @param addressSize 地址长度
 *              --
 *              | QSPI_ADDRESS_8_BITS
 *              | QSPI_ADDRESS_16_BITS
 *              | QSPI_ADDRESS_24_BITS
 *              | QSPI_ADDRESS_32_BITS
 *
 * @param dataMode 数据模式
 *              --
 *              | QSPI_DATA_NONE
 *              | QSPI_DATA_1_LINE
 *              | QSPI_DATA_2_LINE
 *              | QSPI_DATA_4_LINE
 */
void QSPI_Send_CMD( u32 instruction, 
                    u32 address, 
                    u32 dummyCycles, 
                    u32 instructionMode, 
                    u32 addressMode, 
                    u32 addressSize, 
                    u32 dataMode)
{
    QSPI_CommandTypeDef Cmdhandler;

    Cmdhandler.Instruction = instruction;                     // 指令
    Cmdhandler.Address = address;                             // 地址
    Cmdhandler.DummyCycles = dummyCycles;                     // 设置空指令周期数
    Cmdhandler.InstructionMode = instructionMode;             // 指令模式
    Cmdhandler.AddressMode = addressMode;                     // 地址模式
    Cmdhandler.AddressSize = addressSize;                     // 地址长度
    Cmdhandler.DataMode = dataMode;                           // 数据模式
    Cmdhandler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;           // 每次都发送指令
    Cmdhandler.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; // 无交替字节
    Cmdhandler.DdrMode = QSPI_DDR_MODE_DISABLE;               // 关闭DDR模式
    Cmdhandler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&hqspi, &Cmdhandler, 5000);
}

/**
 * @brief QSPI接收指定长度的数据
 * @param buf 接收数据缓冲区首地址
 * @param datalen 要传输的数据长度
 * @return u8 0-正常 其他-错误
 */
u8 QSPI_Receive(u8 *buf, u32 datalen)
{
    hqspi.Instance->DLR = datalen - 1; // 配置数据长度
    if (HAL_QSPI_Receive(&hqspi, buf, 5000) == HAL_OK)
        return 0; // 接收数据
    else
        return 1;
}

/**
 * @brief QSPI发送指定长度的数据
 * @param buf 发送数据缓冲区首地址
 * @param datalen 要发送的数据长度
 * @return u8 0-正常 其他-错误
 */
u8 QSPI_Transmit(u8 *buf, u32 datalen)
{
    hqspi.Instance->DLR = datalen - 1; // 配置数据长度
    if (HAL_QSPI_Transmit(&hqspi, buf, 5000) == HAL_OK)
        return 0; // 发送数据
    else
        return 1;
}