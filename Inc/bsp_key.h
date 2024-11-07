/**
 * @file bsp_key.h
 * @author July (Email: JulyCub@163.com)
 * @brief key driver header file.
 * @version 1.1
 * @date 2023-04-06
 * @update 2024-11-03 -> v1.1 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

#if (__NORMAL_KEY == 1)

#ifdef __BSP_MCU_DEVEBOX_STM32F407VET6

#define BSP_KEY0_PORT GPIOB
#define BSP_KEY0_PIN GPIO_PIN_9

#define BSP_KEY1_PORT GPIOB
#define BSP_KEY1_PIN GPIO_PIN_8

#define BSP_KEY_PRESSED GPIO_PIN_RESET
#define BSP_KEY_RELEASE GPIO_PIN_SET

#define KEY_DEBOUNCE_DELAY 50

void BSP_KEY_Init(void);
GPIO_PinState BSP_KEY_Read(uint8_t key_num);

#endif /* !__BSP_MCU_DEVEBOX_STM32F407VET6 */

#elif (__STATEMAC_KEY == 1)

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

#ifdef __BSP_MCU_DEVEBOX_STM32F407VET6
typedef enum
{
    KEY0 = 0,
    KEY1,
    KEY_NUM
} BSP_KeyList;
#elif defined(__BSP_MCU_LANQIAO_GXCT_STM32G431)
typedef enum
{
    KEY0 = 0,
    KEY1,
    KEY2,
    KEY3,
    KEY_NUM
} BSP_KeyList;
#endif /* !__BSP_MCU_DEVEBOX_STM32F407VET6 */

typedef enum
{
    ACTIVE = 0,
    INACTIVE
}BSP_IoState;

typedef struct
{
    GPIO_TypeDef *GPIOx;
    GPIO_InitTypeDef GPIO_Init;
} BSP_KEY_InitTypeDef;

typedef struct
{
    HAL_LockTypeDef Lock;
    int32_t timstamp;
    BSP_KeyList id;
    GPIO_PinState IO_VALID;
    BSP_IoState io_state;
    BSP_KeyState key_state;
    BSP_KeyEvent key_event;
    BSP_KEY_InitTypeDef Init;

    GPIO_PinState (*ReadPin)(BSP_KEY_InitTypeDef key_init);
} BSP_KEY_HandleTypeDef;

extern BSP_KEY_HandleTypeDef key[KEY_NUM];

void BSP_KEY_Init(void);
void BSP_KEY_StateTransition(void);

#endif /* !__STATEMAC_KEY */

#ifdef __cplusplus
}
#endif /* !__cplusplus */
#endif /* !__BSP_KEY_H */
