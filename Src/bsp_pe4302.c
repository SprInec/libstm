/**
 * @file bsp_pe4302.c
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.07.11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_pe4302.h"

#ifndef PE4302_USING_MULT
/**
 * @brief PE4302 初始化
 */
void BSP_PE4302_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_PIN_2;
  GPIO_InitStructure.Mode = GPIO_Mode_Out_PP;                // 推挽输出
  GPIO_InitStructure.Speed = GPIO_Speed_50MHz;               // IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);                     // 根据设定参数初始化GPIOE
  GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_PIN_2); // PBE 输出高
}

/**
 * @brief PE4302 增益/衰减设置
 * @param db 增益/衰减值
 */
void BSP_PE4302_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db & 0X3F;
  PE_CLK_0;
  PE_LE = 1;
  for (i = 0; i < 6; i++)
  {
    W_DB = (db >> 5);
    if (W_DB == 1)
    {
      PE_DAT_1;
    }
    else
    {
      PE_DAT_0;
    }
    PE_CLK_1;
    db = (db << 1) & 0X3F;
    PE_CLK_0;
  }
  PE_LE = 0;
}
#else
/**
 * @brief PE4302 初始化
 */
void BSP_PE4302_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.Mode = GPIO_Mode_Out_PP;                             // 推挽输出
  GPIO_InitStructure.Speed = GPIO_Speed_50MHz;                            // IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);                                  // 根据设定参数初始化GPIOE
  GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); // PBE 输出高
}

/**
 * @brief PE4302-0 增益/衰减设置
 * @param db 增益/衰减值
 */
void BSP_PE4302_0_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db & 0X3F;

  PE_LE_1_DIS;
  delay_us(500);
  PE_CLK_0;
  delay_us(500);
  PE_LE_0_EN;
  delay_us(500);
  for (i = 0; i < 6; i++)
  {
    W_DB = (db >> 5);
    if (W_DB == 1)
    {
      PE_DAT_1;
    }
    else
    {
      PE_DAT_0;
    }
    delay_us(500);
    PE_CLK_1;
    delay_us(500);
    db = (db << 1) & 0X3F;
    PE_CLK_0;
    delay_us(500);
  }
  PE_LE_0_DIS;
  delay_us(500);
  PE_DAT_0;
}

/**
 * @brief PE4302-1 增益/衰减设置
 * @param db 增益/衰减值
 */
void BSP_PE4302_1_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db & 0X3F;

  PE_LE_0_DIS;
  delay_us(500);
  PE_CLK_0;
  delay_us(500);
  PE_LE_1_EN;
  delay_us(500);
  for (i = 0; i < 6; i++)
  {
    W_DB = (db >> 5);
    if (W_DB == 1)
    {
      PE_DAT_1;
    }
    else
    {
      PE_DAT_0;
    }
    delay_us(500);
    PE_CLK_1;
    delay_us(500);
    db = (db << 1) & 0X3F;
    PE_CLK_0;
    delay_us(500);
  }
  PE_LE_1_DIS;
  delay_us(500);
  PE_DAT_0;
}
#endif /* !PE4302_USING_MULT */
