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

/*! 使用前优先修改以下引脚配置 !*/
#define I2C_SCL PBout(6)
#define I2C_SDA PBout(7)
#define READ_SDA PBin(7)

#define I2C_PORT INA226_I2C_PORT
#define I2C_SCL_PIN INA226_SCL_PIN
#define I2C_SDA_PIN INA226_SDA_PIN

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
#endif 

// IO方向设置
#define SDA_IN() {                                               \
        I2C_PORT->MODER &= ~((uint32_t)0x3 << (7 * 2));        \
        I2C_PORT->MODER |= ((uint32_t)0x0 << (7 * 2));         \
}
    
#define SDA_OUT() {                                             \
        I2C_PORT->MODER &= ~((uint32_t)0x3 << (7 * 2));        \
        I2C_PORT->MODER |= ((uint32_t)0x1 << (7 * 2));         \
}

void BSP_I2C_Init(GPIO_TypeDef *I2C_Port, uint16_t I2C_SCL_Pin, uint16_t I2C_SDA_Pin);
void BSP_I2C_Start(void);
void BSP_I2C_Stop(void);
void BSP_I2C_Send_Byte(u8 txd);
u8 BSP_I2C_Read_Byte(unsigned char ack);
u8 BSP_I2C_Wait_Ack(uint8_t id);
void BSP_I2C_Ack(void);
void BSP_I2C_NAck(void);

#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __BSP_I2C_H__ */
