/**
 * @file bsp_dht20.c
 * @author July (Email: JulyCub@163.com)
 * @brief DHT20 Hardware
 * @version 0.1
 * @date 2023.08.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_dht20.h"

void DHT20_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __BSP_RCC_GPIO_ENABLE(DHT20_GPIO_PORT);

    GPIO_InitStruct.Pin = DHT20_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(DHT20_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SDA_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = DHT20_SCL_PIN;
    HAL_GPIO_Init(DHT20_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SCL_PIN, GPIO_PIN_SET);
}

void DHT20_SDA_Pin_Output_High(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = DHT20_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT20_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SDA_PIN, GPIO_PIN_SET);
}

void DHT20_SDA_Pin_Output_Low(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = DHT20_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT20_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SDA_PIN, GPIO_PIN_RESET);
}

void DHT20_SDA_Pin_IN_FLOATING(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = DHT20_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(DHT20_GPIO_PORT, &GPIO_InitStruct);
}

void DHT20_SCL_Pin_Output_High(void)
{
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SCL_PIN, GPIO_PIN_SET);
}

void DHT20_SCL_Pin_Output_Low(void)
{
    HAL_GPIO_WritePin(DHT20_GPIO_PORT, DHT20_SCL_PIN, GPIO_PIN_RESET);
}

void DHT20_I2C_Start(void) // I2C主机发送START信号
{
    DHT20_SDA_Pin_Output_High();
    delay_us(8);
    DHT20_SCL_Pin_Output_High();
    delay_us(8);
    DHT20_SDA_Pin_Output_Low();
    delay_us(8);
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
}

void DHT20_WR_Byte(uint8_t Byte)
{
    uint8_t Data, N, i;
    Data = Byte;
    i = 0x80;
    for (N = 0; N < 8; N++)
    {
        DHT20_SCL_Pin_Output_Low();
        delay_us(4);
        if (i & Data)
        {
            DHT20_SDA_Pin_Output_High();
        }
        else
        {
            DHT20_SDA_Pin_Output_Low();
        }

        DHT20_SCL_Pin_Output_High();
        delay_us(4);
        Data <<= 1;
    }
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    DHT20_SDA_Pin_IN_FLOATING();
    delay_us(8);
}

uint8_t DHT20_RD_Byte(void)
{
    uint8_t Byte, i, a;
    Byte = 0;
    DHT20_SCL_Pin_Output_Low();
    DHT20_SDA_Pin_IN_FLOATING();
    delay_us(8);
    for (i = 0; i < 8; i++)
    {
        DHT20_SCL_Pin_Output_High();
        delay_us(5);
        a = 0;
        if (HAL_GPIO_ReadPin(DHT20_GPIO_PORT, DHT20_SDA_PIN))
            a = 1;
        Byte = (Byte << 1) | a;
        DHT20_SCL_Pin_Output_Low();
        delay_us(5);
    }
    DHT20_SDA_Pin_IN_FLOATING();
    delay_us(8);
    return Byte;
}

uint8_t DHT20_Receive_ACK(void)
{
    uint16_t CNT;
    CNT = 0;
    DHT20_SCL_Pin_Output_Low();
    DHT20_SDA_Pin_IN_FLOATING();
    delay_us(8);
    DHT20_SCL_Pin_Output_High();
    delay_us(8);
    while ((HAL_GPIO_ReadPin(DHT20_GPIO_PORT, DHT20_SDA_PIN)) && CNT < 100)
        CNT++;
    if (CNT == 100)
    {
        return 0;
    }
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    return 1;
}

void DHT20_Send_ACK(void)
{
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    DHT20_SDA_Pin_Output_Low();
    delay_us(8);
    DHT20_SCL_Pin_Output_High();
    delay_us(8);
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    DHT20_SDA_Pin_IN_FLOATING();
    delay_us(8);
}

void DHT20_Send_NOT_ACK(void)
{
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    DHT20_SDA_Pin_Output_High();
    delay_us(8);
    DHT20_SCL_Pin_Output_High();
    delay_us(8);
    DHT20_SCL_Pin_Output_Low();
    delay_us(8);
    DHT20_SDA_Pin_Output_Low();
    delay_us(8);
}

void DHT20_Stop_I2C(void)
{
    DHT20_SDA_Pin_Output_Low();
    delay_us(8);
    DHT20_SCL_Pin_Output_High();
    delay_us(8);
    DHT20_SDA_Pin_Output_High();
    delay_us(8);
}

uint8_t DHT20_Read_Status(void)
{
    uint8_t Byte_first;

    DHT20_I2C_Start();
    DHT20_WR_Byte(0x71);
    DHT20_Receive_ACK();
    Byte_first = DHT20_RD_Byte();
    DHT20_Send_NOT_ACK();
    DHT20_Stop_I2C();
    return Byte_first;
}

void DHT20_SendAC(void)
{
    DHT20_I2C_Start();
    DHT20_WR_Byte(0x70);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0xac);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0x33);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0x00);
    DHT20_Receive_ACK();
    DHT20_Stop_I2C();
}

/**
 * @brief CRC校验
 *
 * @attention 校验类型:CRC8/MAXIM
 * @function X8+X5+X4+1
 * @poly 0011 0001 0x31 | 1000 1100 0x8c
 * @param message 待校验数据
 * @param Num 数据长度
 * @return uint8_t CRC校验码
 */
uint8_t Calc_CRC8(uint8_t *message, uint8_t Num)
{
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    for (byte = 0; byte < Num; byte++)
    {
        crc ^= (message[byte]);
        for (i = 8; i > 0; --i)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

/**
 * @brief 读取DHT20的温度和湿度数据(无CRC校验)
 * @param ct ct[0]-湿度|ct[1]-温度
 */
void BSP_DHT20_Read_CTdata(uint32_t *ct)
{
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;

    DHT20_SDA_PIN();
    delay_ms(80);
    cnt = 0;
    // 直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    while (((DHT20_Read_Status() & 0x80) == 0x80))
    {
        delay_us(1508);
        if (cnt++ >= 100)
        {
            break;
        }
    }
    DHT20_I2C_Start();
    DHT20_WR_Byte(0x71);
    DHT20_Receive_ACK();
    // 状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；
    // 状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    Byte_1th = DHT20_RD_Byte();
    DHT20_Send_ACK();
    Byte_2th = DHT20_RD_Byte(); // 湿度
    DHT20_Send_ACK();
    Byte_3th = DHT20_RD_Byte(); // 湿度
    DHT20_Send_ACK();
    Byte_4th = DHT20_RD_Byte(); // 湿度/温度
    DHT20_Send_ACK();
    Byte_5th = DHT20_RD_Byte(); // 温度
    DHT20_Send_ACK();
    Byte_6th = DHT20_RD_Byte(); // 温度
    DHT20_Send_NOT_ACK();
    DHT20_Stop_I2C();

    RetuData = (RetuData | Byte_2th) << 8;
    RetuData = (RetuData | Byte_3th) << 8;
    RetuData = (RetuData | Byte_4th);
    RetuData = RetuData >> 4;
    ct[0] = RetuData * 1000 / 1024 / 1024; // 湿度
    RetuData = 0;
    RetuData = (RetuData | Byte_4th) << 8;
    RetuData = (RetuData | Byte_5th) << 8;
    RetuData = (RetuData | Byte_6th);
    RetuData = RetuData & 0xfffff;
    ct[1] = RetuData * 2000 / 1024 / 1024 - 500; // 温度
}

/**
 * @brief 读取AHT20的温度和湿度数据(带CRC校验)
 * @param ct ct[0]-湿度|ct[1]-温度
 */
void BSP_DHT20_Read_CTdata_CRC(uint32_t *ct)
{
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    volatile uint8_t Byte_7th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;
    uint8_t CTDATA[6] = {0}; // 用于CRC传递数组

    DHT20_SendAC(); // 向AHT20发送AC命令
    delay_ms(80);   // 延时80ms左右
    cnt = 0;
    // 直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    while (((DHT20_Read_Status() & 0x80) == 0x80))
        /
        {
            delay_us(1508);
            if (cnt++ >= 100)
            {
                break;
            }
        }

    DHT20_I2C_Start();
    DHT20_WR_Byte(0x71);
    DHT20_Receive_ACK();
    CTDATA[0] = Byte_1th = DHT20_RD_Byte(); // 状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    DHT20_Send_ACK();
    CTDATA[1] = Byte_2th = DHT20_RD_Byte(); // 湿度
    DHT20_Send_ACK();
    CTDATA[2] = Byte_3th = DHT20_RD_Byte(); // 湿度
    DHT20_Send_ACK();
    CTDATA[3] = Byte_4th = DHT20_RD_Byte(); // 湿度/温度
    DHT20_Send_ACK();
    CTDATA[4] = Byte_5th = DHT20_RD_Byte(); // 温度
    DHT20_Send_ACK();
    CTDATA[5] = Byte_6th = DHT20_RD_Byte(); // 温度
    DHT20_Send_ACK();
    Byte_7th = DHT20_RD_Byte(); // CRC数据
    DHT20_Send_NOT_ACK();       // 注意: 最后是发送NAK
    DHT20_Stop_I2C();

    if (Calc_CRC8(CTDATA, 6) == Byte_7th)
    {
        RetuData = (RetuData | Byte_2th) << 8;
        RetuData = (RetuData | Byte_3th) << 8;
        RetuData = (RetuData | Byte_4th);
        RetuData = RetuData >> 4;
        ct[0] = RetuData * 1000 / 1024 / 1024; // 湿度
        RetuData = 0;
        RetuData = (RetuData | Byte_4th) << 8;
        RetuData = (RetuData | Byte_5th) << 8;
        RetuData = (RetuData | Byte_6th);
        RetuData = RetuData & 0xfffff;
        ct[1] = RetuData * 2000 / 1024 / 1024 - 500; // 温度
    }
    else
    {
        ct[0] = 0x00;
        ct[1] = 0x00; // 校验错误返回值，客户可以根据自己需要更改
    }
}

/**
 * @brief 重置寄存器
 * @param addr 寄存器地址
 */
void JH_Reset_REG(uint8_t addr)
{
    uint8_t Byte_first, Byte_second, Byte_third, Byte_fourth;
    DHT20_I2C_Start();
    DHT20_WR_Byte(0x70); // 原来是0x70
    DHT20_Receive_ACK();
    DHT20_WR_Byte(addr);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0x00);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0x00);
    DHT20_Receive_ACK();
    DHT20_Stop_I2C();

    delay_ms(5); // 延时5ms左右
    DHT20_I2C_Start();
    DHT20_WR_Byte(0x71);
    DHT20_Receive_ACK();
    Byte_first = DHT20_RD_Byte();
    DHT20_Send_ACK();
    Byte_second = DHT20_RD_Byte();
    DHT20_Send_ACK();
    Byte_third = DHT20_RD_Byte();
    DHT20_Send_NOT_ACK();
    DHT20_Stop_I2C();

    delay_ms(10); // 延时10ms左右
    DHT20_I2C_Start();
    DHT20_WR_Byte(0x70);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(0xB0 | addr); // 寄存器命令
    DHT20_Receive_ACK();
    DHT20_WR_Byte(Byte_second);
    DHT20_Receive_ACK();
    DHT20_WR_Byte(Byte_third);
    DHT20_Receive_ACK();
    DHT20_Stop_I2C();

    Byte_second = 0x00;
    Byte_third = 0x00;
}

void DHT20_Start_Init(void)
{
    JH_Reset_REG(0x1b);
    JH_Reset_REG(0x1c);
    JH_Reset_REG(0x1e);
}

/**
 * @brief DHT20初始化函数
 */
void BSP_DHT20_Init(void)
{
    DHT20_I2C_Init();
    DHT20_Start_Init();
}
