/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2023.08.19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_user.h"

void bsp_init(void)
{
    BSP_LED_Init();
    delay_init();
}
