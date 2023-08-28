/**
 * @file bsp_sccb.h
 * @author July (Email: JulyCub@163.com)
 * @brief SCCB Driver
 * @version 0.1
 * @date 2023.07.16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __BSP_SCCB_H
#define __BSP_SCCB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "bsp_config.h"

#define SCCB_SCL(n) (n ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)) // SCL--PB10
#define SCCB_SDA(n) (n ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)) // SDA--PB11

#define SCCB_READ_SDA HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)
#define SCCB_ID 0X60

void SCCB_Init(void);
void SCCB_SDA_IN(void);  
void SCCB_SDA_OUT(void); 
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg, u8 data);
u8 SCCB_RD_Reg(u8 reg);

#ifdef __cplusplus
}
#endif
#endif /* !BSP_SCCB_H */