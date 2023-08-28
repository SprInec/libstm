/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief User Functions
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

BSP_WeatherData_TypeDef wet_data = {
    .uv = 0,
    .ct[0] = 0,
    .ct[1] = 0,
    .temperature = 0
};
BSP_DHT11Data_TypeDef dht11_data = {0};

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        20

/**
 * @brief DS18B20
 */
static rt_thread_t ds18b20 = RT_NULL;
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

/**
 * @brief DHT11
 */
static rt_thread_t dht11 = RT_NULL;
static void dht11_thread_entry(void *parameter)
{
    BSP_DHT11_Init();
    while (1)
    {
        BSP_DHT11_ReadData(&dht11_data);
        delay_ms(10);
        rt_kprintf("\ndht11->temp: %d.%d", dht11_data.temp_int, dht11_data.temp_deci);
        rt_kprintf("\ndht11->humi: %d.%d", dht11_data.humi_int, dht11_data.humi_deci);
        delay_ms(1000);
    }
}

/**
 * @brief DHT20
 */
static rt_thread_t dht20 = RT_NULL;
static void dht20_thread_entry(void *parameter)
{
    BSP_DHT20_Init();
    while(1)
    {
        BSP_DHT20_Read_CTdata_CRC(wet_data.ct);
        rt_kprintf("\ndht20->temp: %d", wet_data.ct[1]);
        rt_kprintf("\ndht20->humi: %d", wet_data.ct[0]);
        delay_ms(1000);
    }
}

static rt_thread_t s12sd = RT_NULL;
static void s12sd_thread_entry(void *parameter)
{
    BSP_S12SD_Init();
    while (1)
    {
        wet_data.uv =  BSP_S12SD_UV_Index(BSP_S12SD_Read());
        rt_kprintf("\ns12sd->uv: %d", wet_data.uv);
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


    dht20 = rt_thread_create("dht20",
                             dht20_thread_entry,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);
    if (dht20 != RT_NULL)
        rt_thread_startup(dht20);

    s12sd = rt_thread_create("s12sd",
                             s12sd_thread_entry,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);
    if (s12sd != RT_NULL)
        rt_thread_startup(s12sd);
}
MSH_CMD_EXPORT(thread_start, thread start);

/**
 * @brief main function
 * @return int
 */
int main(void)
{
    BSP_LED_Init();
    thread_start();
    while (1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        delay_ms(500);
    }
}
