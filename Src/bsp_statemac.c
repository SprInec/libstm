/**
 * @file bsp_statemac.c
 * @author July (Email: JulyCub@163.com)
 * @brief state machine
 * @version 0.1
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_statemac.h"

BSP_KEY_HandleTypeDef key[KEY_NUM];

/**
 * @brief 按键引脚初始化
 *
 */
void BSP_KEY_GPIO_Init(void)
{
    BSP_KEY_InitTypeDef key_init[KEY_NUM];

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    key_init[0].GPIOx = GPIOB;
    key_init[1].GPIOx = GPIOB;
    key_init[2].GPIOx = GPIOB;
    key_init[3].GPIOx = GPIOB;

    key_init[0].GPIO_Init->Pin = GPIO_PIN_8;
    key_init[0].GPIO_Init->Mode = GPIO_MODE_INPUT;
    key_init[0].GPIO_Init->Pull = GPIO_NOPULL;
    key[0].Init = key_init[0];
    HAL_GPIO_Init(key_init[0].GPIOx, key_init[0].GPIO_Init);

    key_init[1].GPIO_Init->Pin = GPIO_PIN_9;
    key_init[1].GPIO_Init->Mode = GPIO_MODE_INPUT;
    key_init[1].GPIO_Init->Pull = GPIO_NOPULL;
    key[1].Init = key_init[1];
    HAL_GPIO_Init(key_init[1].GPIOx, key_init[1].GPIO_Init);

    key_init[2].GPIO_Init->Pin = GPIO_PIN_2;
    key_init[2].GPIO_Init->Mode = GPIO_MODE_INPUT;
    key_init[2].GPIO_Init->Pull = GPIO_NOPULL;
    key[2].Init = key_init[2];
    HAL_GPIO_Init(key_init[2].GPIOx, key_init[2].GPIO_Init);

    key_init[3].GPIO_Init->Pin = GPIO_PIN_3;
    key_init[3].GPIO_Init->Mode = GPIO_MODE_INPUT;
    key_init[3].GPIO_Init->Pull = GPIO_NOPULL;
    key[3].Init = key_init[3];
    HAL_GPIO_Init(key_init[3].GPIOx, key_init[3].GPIO_Init);
}

/**
 * @brief 按键初始化
 *
 */
void BSP_KEY_Init(void)
{
    BSP_KEY_GPIO_Init();
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        key[i].id = i;
        key[i].Lock = HAL_UNLOCKED;
        if (key[i].Init.GPIO_Init->Pull == GPIO_PULLUP)
            key[i].IO_VALID = GPIO_PIN_RESET;
        else
            key[i].IO_VALID = GPIO_PIN_SET;
				key[i].io_state = INACTIVE;
        key[i].key_state= KEY_IDLE;
        key[i].key_event = KEY_NULL;
        key[i].timstamp = 0;
        key[i].ReadPin = BSP_Key_ReadPin;
    }
}

/**
 * @brief 按键状态捕获
 *
 */
static void BSP_KEY_Capture(void)
{
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        if (key[i].Lock == HAL_LOCKED)
            continue;
        if (key[i].ReadPin(key[i].Init) == key[i].IO_VALID)
            key[i].io_state = ACTIVE;
        else
            key[i].io_state = INACTIVE;
    }
}

/**
 * @brief 按键状态转换
 * 
 */
void BSP_KEY_StateTransition(void)
{
    BSP_KEY_Capture();
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        if (key[i].Lock == HAL_LOCKED)
            continue;
        switch (key[i].key_state)
        {
        case KEY_IDLE:
            if (key[i].io_state == ACTIVE)
            {
                key[i].key_state = KEY_SURE;
                key[i].key_event = KEY_NULL;
            }
            else
            {
                key[i].key_event = KEY_NULL;
            }
            break;
        case KEY_SURE:
            if (key[i].io_state == ACTIVE)
            {
                key[i].key_state = KEY_PRESSED;
                key[i].key_event = KEY_PRESS;
            }
            else
            {
                key[i].key_state = KEY_IDLE;
                key[i].key_event = KEY_NULL;
            }
            break;
        case KEY_PRESSED:
            if (key[i].io_state == INACTIVE)
            {
                key[i].key_state = KEY_WAITING;
                key[i].key_event = KEY_RELEASE;
                key[i].timstamp = BSP_KEY_DOUB_TIMEOUT;
            }
            else if (key[i].io_state == ACTIVE &&
                    ++key[i].timstamp >= BSP_KEY_LONG_TIMEOUT)
            {
                key[i].key_state = KEY_LONG_PRESSED;
                key[i].key_event = KEY_LONG_PRESS;
                key[i].timstamp = 0;
            }
            else
            {
                key[i].key_event = KEY_NULL;
            }
            break;
        case KEY_WAITING:
            if (key[i].io_state == ACTIVE && (key[i].timstamp--))
            {
                key[i].key_state = KEY_DOUB_PRESSED;
                key[i].key_event = KEY_DOUB_PRESS;
                key[i].timstamp = 0;
            }
            else if (!key[i].timstamp)
            {
                key[i].key_state = KEY_IDLE;
                key[i].key_event = KEY_NULL;
                key[i].timstamp = 0;
            }
            else
            {
                key[i].key_event = KEY_NULL;
            }
            break;
        case KEY_LONG_PRESSED:
            if (key[i].io_state == INACTIVE)
            {
                key[i].key_state = KEY_IDLE;
                key[i].key_event = KEY_LONG_RELEASE;
            }
            else if (key[i].io_state == ACTIVE &&
                    ++key[i].timstamp >= BSP_KEY_LONG_TIMEOUT)
            {
                key[i].key_state = KEY_LONG_PRESSED;
                key[i].timstamp = 0;
            }
            else
            {
                key[i].key_event = KEY_NULL;
            }
            break;
        case KEY_DOUB_PRESSED:
            
            default: break;
        }
    }
}
