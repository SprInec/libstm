/**
 * @file bsp_user.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date xxx
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _BSP_USER_H_
#define _BSP_USER_H_

#include "bsp_config.h"

/**
 * @brief SPCDDS 结构体
 */
typedef struct SPCDDS
{
    uint8_t mode;
    float freq;
    float ampl;
}BSP_SPCDDS_TypeDef;


void BSP_Init(void);
void BSP_UART_CONTR(void);

#endif /* _BSP_USER_H_ */