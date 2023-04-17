/**
 * @file bsp_statemac.h
 * @author July (Email: JulyCub@163.com)
 * @brief state machine 
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_STATEMAC_H__
#define __BSP_STATEMAC_H__

#include "bsp_config.h"

#ifndef __STATEMAC_KEY

#define BSP_KEY_LONG_TIMEOUT 20
#define BSP_KEY_DOUB_TIMEOUT 5

typedef enum
{
    KEY_IDLE = 0,
    KEY_SURE,
    KEY_WAITING,
    KEY_PRESSED,
    KEY_LONG_PRESSED,
    KEY_DOUB_PRESSED,
} BSP_KeyState;

typedef enum
{
    KEY_NULL = 0,
    KEY_PRESS,
    KEY_RELEASE,
    KEY_DOUB_PRESS,
    KEY_LONG_PRESS,
    KEY_LONG_RELEASE
} BSP_KeyEvent;

typedef enum
{
    KEY0 = 0,
    KEY1,
    KEY2,
    KEY3,
    KEY_NUM
} BSP_KeyList;

typedef enum
{
    ACTIVE = 0,
    INACTIVE
}BSP_IoState;

typedef struct
{
    GPIO_TypeDef *GPIOx;
    GPIO_InitTypeDef *GPIO_Init;
} BSP_KEY_InitTypeDef;

typedef struct
{
    HAL_LockTypeDef Lock;
    int8_t timstamp;
    BSP_KeyList id;
    GPIO_PinState IO_VALID;
    BSP_IoState io_state;
    BSP_KeyState key_state;
    BSP_KeyEvent key_event;
    BSP_KEY_InitTypeDef Init;

    GPIO_PinState (*ReadPin)(BSP_KEY_InitTypeDef key_init);
} BSP_KEY_HandleTypeDef;

static GPIO_PinState BSP_Key_ReadPin(BSP_KEY_InitTypeDef key_init)
{
    return HAL_GPIO_ReadPin(key_init.GPIOx, key_init.GPIO_Init->Pin);
}

void BSP_KEY_Init(void);
void BSP_KEY_StateTransition(void);

#endif /* __STATEMAC_KEY */
#endif /* __BSP_STATEMAC_H */
