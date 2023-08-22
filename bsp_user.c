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
#include "rtthread.h"
#include "gpio.h"
#include "usart.h"

BSP_WeatherData_TypeDef wet_data;
BSP_DHT11Data_TypeDef dht11_data;

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        20

static rt_thread_t ds18b20 = RT_NULL;
/**
 * @brief ds18b20 thread entry function
 * @param parameter 
 */
static void ds18b20_thread_entry(void *parameter)
{
    DS18B20_Init();
    while(1)
    {
        wet_data.temperature = DS18B20_Get_Temp();
        rt_uint16_t data = (rt_uint16_t)wet_data.temperature;
        rt_kprintf("\nds18b20->temp: %d", data);
        delay_ms(1000);
    }   
}

static rt_thread_t dht11 = RT_NULL;
/**
 * @brief dht11 thread entry function
 * @param parameter 
 */
static void dht11_thread_entry(void *parameter)
{
    BSP_DHT11_Init();
    while (1)
    {
        BSP_DHT11_ReadData(&dht11_data);
        rt_kprintf("\ndht11->temp: %d.%d", dht11_data.temp_int, dht11_data.temp_deci);
        rt_kprintf("\ndht11->humi: %d.%d", dht11_data.humi_int, dht11_data.humi_deci);
        delay_ms(1000);
    }
}

void thread_start(void)
{
    ds18b20 = rt_thread_create("ds18b20",
                               ds18b20_thread_entry,
                               RT_NULL,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY,
                               THREAD_TIMESLICE);

    if (ds18b20 != RT_NULL)
        rt_thread_startup(ds18b20);

    dht11 = rt_thread_create("dht11",
                             dht11_thread_entry,        
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    if (dht11 != RT_NULL)
        rt_thread_startup(dht11);
}
MSH_CMD_EXPORT(thread_start, thread start);

/**
 * @brief main function
 * @return int
 */
int main(void)
{
    BSP_LED_Init();
    while (1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        delay_ms(200);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        delay_ms(200);
    }
}
