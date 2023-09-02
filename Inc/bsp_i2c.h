/**
 * @file bsp_i2c.hpp
 * @author July (Email: JulyCub@163.com)
 * @brief I2C Hardware
 * @version 0.1
 * @date 2023.08.28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__
#include "bsp_config.h"

#define DELAY_TIME 20
#ifdef __cplusplus
#include <string>

class BSP_I2C_CLASS
{
private:
    std::string device;
    GPIO_TypeDef *Port;
    GPIO_InitTypeDef SDA_Init;
    GPIO_InitTypeDef SCL_Init;
public:
    void Init(const std::string &name, GPIO_TypeDef *port, uint16_t sda_pin, uint16_t scl_pin);
    void DeInit(void);
    void SDA_Input_Mode(void);
    void SDA_Output_Mode(void);
    void SDA_Output(uint16_t val);
    uint8_t SDA_Input(void);
    void SCL_Output(uint16_t val);
    void Start(void);
    void Stop(void);
    uint8_t WaitACK(void);
    uint8_t WaitNotACK(void);
    void SendACK(void);
    void SendNotACK(void);
    void SendByte(uint8_t byte);
    uint8_t ReceiveByte(void);
    void delay(uint16_t n);
};
#else

#endif /* __cplusplus */
#endif /* __BSP_I2C_H__ */
