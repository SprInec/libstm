/**
 * @file bsp_i2c.cpp
 * @author July (Email: JulyCub@163.com)
 * @brief i2C Hardware
 * @version 0.1
 * @date 2023.08.28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bsp_i2c.hpp"

#define DELAY_TIME 20

/**
 * @brief 初始化I2C
 *
 * @param name 设备名称
 * @param port GPIO端口号
 * @param sda_pin SDA引脚
 * @param scl_pin SCL引脚
 */
void BSP_I2C_CLASS::Init(const std::string &name,
                         GPIO_TypeDef port,
                         uint16_t sda_pin,
                         uint16_t scl_pin)
{
  device = name;
  Port = port;

  __BSP_RCC_GPIO_ENABLE(Port);

  SDA_Init.Pin = sda_pin;
  SDA_Init.Mode = GPIO_MODE_OUTPUT_PP;
  SDA_Init.Pull = GPIO_PULLUP;
  SDA_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Port, &SDA_Init);

  SCL_Init.Pin = scl_pin;
  SCL_Init.Mode = GPIO_MODE_OUTPUT_PP;
  SCL_Init.Pull = GPIO_PULLUP;
  SCL_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Port, &SCL_Init);
}

/**
 * @brief DeInit I2C
 */
void BSP_I2C_CLASS::DeInit(void)
{
  HAL_GPIO_DeInit(Port, SDA_Init.Pin);
  HAL_GPIO_DeInit(Port, SCL_Init.Pin);
}

/**
 * @brief SDA线输入模式配置
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::SDA_Input_Mode(void)
{
  SDA_Init.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(Port, &SDA_Init);
}

/**
 * @brief SDA线输出模式配置
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::SDA_Output_Mode(void)
{
  SDA_Init.Mode = GPIO_MODE_OUTPUT_OD;
  HAL_GPIO_Init(Port, &SDA_Init);
}

/**
 * @brief SDA线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void BSP_I2C_CLASS::SDA_Output(uint16_t val)
{
  if (val)
  {
    Port->BSRR |= SDA_Init.Pin;
  }
  else
  {
    Port->ODR |= SDA_Init.Pin;
  }
}

/**
 * @brief SCL线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void BSP_I2C_CLASS::SCL_Output(uint16_t val)
{
  if (val)
  {
    Port->BSRR |= SCL_Init.Pin;
  }
  else
  {
    Port->ODR |= SCL_Init.Pin;
  }
}

/**
 * @brief SDA输入一位
 * @param None
 * @retval GPIO读入一位
 */
uint8_t BSP_I2C_CLASS::SDA_Input(void)
{
  if (HAL_GPIO_ReadPin(Port, SDA_Init.Pin) == GPIO_PIN_SET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief I2C的短暂延时
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::delay(uint16_t n)
{
  uint32_t i;
  for (i = 0; i < n; ++i)
    ;
}

/**
 * @brief I2C起始信号
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::Start(void)
{
  BSP_I2C_CLASS::SDA_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SDA_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
}

/**
 * @brief I2C结束信号
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::Stop(void)
{
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SDA_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SDA_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
}

/**
 * @brief I2C等待确认信号
 * @param None
 * @retval None
 */
uint8_t BSP_I2C_CLASS::WaitACK(void)
{
  unsigned short cErrTime = 5;

  BSP_I2C_CLASS::SDA_Input_Mode();
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  while (BSP_I2C_CLASS::SDA_Input())
  {
    cErrTime--;
    BSP_I2C_CLASS::delay(DELAY_TIME);
    if (0 == cErrTime)
    {
      BSP_I2C_CLASS::SDA_Output_Mode();
      BSP_I2C_CLASS::Stop();
      return ERROR;
    }
  }
  BSP_I2C_CLASS::SDA_Output_Mode();
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  return SUCCESS;
}

/**
 * @brief I2C等待NO ACK信号
 * @param None
 * @retval None
 */
uint8_t BSP_I2C_CLASS::WaitNotACK(void)
{
  unsigned short cErrTime = 5;
  BSP_I2C_CLASS::SDA_Input_Mode();
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  while (!BSP_I2C_CLASS::SDA_Input())
  {
    cErrTime--;
    BSP_I2C_CLASS::delay(DELAY_TIME);
    if (0 == cErrTime)
    {
      BSP_I2C_CLASS::SDA_Output_Mode();
      BSP_I2C_CLASS::Stop();
      return ERROR;
    }
  }
  BSP_I2C_CLASS::SDA_Output_Mode();
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  return SUCCESS;
}

/**
 * @brief I2C发送确认信号
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::SendACK(void)
{
  BSP_I2C_CLASS::SDA_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
}

/**
 * @brief I2C发送非确认信号
 * @param None
 * @retval None
 */
void BSP_I2C_CLASS::SendNotACK(void)
{
  BSP_I2C_CLASS::SDA_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(1);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
}

/**
 * @brief I2C发送一个字节
 * @param byte 需要发送的字节
 * @retval None
 */
void BSP_I2C_CLASS::SendByte(uint8_t byte)
{
  unsigned char i = 8;
  while (i--)
  {
    BSP_I2C_CLASS::SCL_Output(0);
    BSP_I2C_CLASS::delay(DELAY_TIME);
    BSP_I2C_CLASS::SDA_Output(byte & 0x80);
    BSP_I2C_CLASS::delay(DELAY_TIME);
    byte += byte;
    BSP_I2C_CLASS::delay(DELAY_TIME);
    BSP_I2C_CLASS::SCL_Output(1);
    BSP_I2C_CLASS::delay(DELAY_TIME);
  }
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
}

/**
 * @brief I2C接收一个字节
 * @param None
 * @retval 接收到的字节
 */
uint8_t BSP_I2C_CLASS::ReceiveByte(void)
{
  unsigned char i = 8;
  unsigned char byte = 0;
  BSP_I2C_CLASS::SDA_Input_Mode();
  while (i--)
  {
    byte += byte;
    BSP_I2C_CLASS::SCL_Output(0);
    BSP_I2C_CLASS::delay(DELAY_TIME);
    BSP_I2C_CLASS::delay(DELAY_TIME);
    BSP_I2C_CLASS::SCL_Output(1);
    BSP_I2C_CLASS::delay(DELAY_TIME);
    byte |= BSP_I2C_CLASS::SDA_Input();
  }
  BSP_I2C_CLASS::SCL_Output(0);
  BSP_I2C_CLASS::delay(DELAY_TIME);
  BSP_I2C_CLASS::SDA_Output_Mode();
  return byte;
}
