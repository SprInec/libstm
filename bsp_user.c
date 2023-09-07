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
#include "string.h"

BSP_WeatherData_TypeDef wet_data = {
    .uv = 0,
    .ct[0] = 0,
    .ct[1] = 0,
    .PM2_5 = 0.0,
    .temperature = 0,
    .pressure = 0.0
};
BSP_DHT11Data_TypeDef dht11_data = {0};

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        20

/**
 * @brief DS18B20 温度信息采集
 */
static rt_thread_t ds18b20 = RT_NULL;
static void ds18b20_thread_entry(void *parameter)
{
    DS18B20_Init();
    rt_kprintf("\n- DS18B20 Start Successful.");
    while(1)
    {
        wet_data.temperature = DS18B20_Get_Temp();
        rt_uint16_t data = (rt_uint16_t)wet_data.temperature;
        rt_kprintf("\nds18b20->temp: %d", data);
        delay_ms(SENSOR_DELAY_TIME);
    }   
}

/**
 * @brief DHT11 温度/湿度信息采集
 */
static rt_thread_t dht11 = RT_NULL;
static void dht11_thread_entry(void *parameter)
{
    BSP_DHT11_Init();
    rt_kprintf("\n- DHT11 Start Successful.");
    while (1)
    {
        BSP_DHT11_ReadData(&dht11_data);
        delay_ms(10);
        rt_kprintf("\ndht11->temp: %d.%d", dht11_data.temp_int, dht11_data.temp_deci);
        rt_kprintf("\ndht11->humi: %d.%d", dht11_data.humi_int, dht11_data.humi_deci);
        delay_ms(SENSOR_DELAY_TIME);

        // BSP_DHT11_ReadData(wet_data.dht11_data);
        // delay_ms(10);
        // rt_kprintf("\ndht11->temp: %d.%d", wet_data.dht11_data->temp_int, wet_data.dht11_data->temp_deci);
        // rt_kprintf("\ndht11->humi: %d.%d", wet_data.dht11_data->humi_int, wet_data.dht11_data->humi_deci);
        // delay_ms(1000);
    }
}

/**
 * @brief DHT20 温度/湿度信息采集
 */
static rt_thread_t dht20 = RT_NULL;
static void dht20_thread_entry(void *parameter)
{
    BSP_DHT20_Init();
    rt_kprintf("\n- DHT20 Start Successful.");
    while(1)
    {
        BSP_DHT20_Read_CTdata_CRC(wet_data.ct);
        rt_kprintf("\ndht20->temp: %d", wet_data.ct[1]);
        rt_kprintf("\ndht20->humi: %d", wet_data.ct[0]);
        delay_ms(SENSOR_DELAY_TIME);
    }
}

/**
 * @brief 紫外线辐射信息采集
 */
static rt_thread_t s12sd = RT_NULL;
static void s12sd_thread_entry(void *parameter)
{
    BSP_S12SD_Init();
    rt_kprintf("\n- S12SD Start Successful.");
    while (1)
    {
        wet_data.uv =  BSP_S12SD_UV_Index(BSP_S12SD_Read());
        rt_kprintf("\ns12sd->uv: %d", wet_data.uv);
        delay_ms(SENSOR_DELAY_TIME);
    }
}

/**
 * @brief 大气压强数据采集
 */
static rt_thread_t _bmp280 = RT_NULL;
static void bmp280_thread_entry(void *parameter)
{
    BSP_BMP280_Init();
    rt_kprintf("\n- BMP280 Start Successful.");
    while (1)
    {
        wet_data.pressure = BSP_BMP280_Get_Pressure();
        rt_kprintf("\nbmp280->P: %d",(uint16_t)wet_data.pressure);
        delay_ms(SENSOR_DELAY_TIME);
    }
}

/**
 * @brief PM2.5传感器
 */
static rt_thread_t gp2y = RT_NULL;
static void gp2y_thread_entry(void *parameter)
{
    BSP_GP2Y_Init();
    rt_kprintf("\n- GP2Y Start Successful.");
    while(1)
    {
        wet_data.PM2_5 = GP2Y_GetDens();
        rt_kprintf("\ngp2y->PM2.5: %d", (uint16_t)(wet_data.PM2_5));
        delay_ms(SENSOR_DELAY_TIME);
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

    _bmp280 = rt_thread_create("bmp280",
                              bmp280_thread_entry,
                              RT_NULL,
                              THREAD_STACK_SIZE,
                              THREAD_PRIORITY,
                              THREAD_TIMESLICE);
    if (_bmp280 != RT_NULL)
        rt_thread_startup(_bmp280);

    gp2y = rt_thread_create("gp2y",
                            gp2y_thread_entry,
                            RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY,
                            THREAD_TIMESLICE);
    if (gp2y != RT_NULL)
        rt_thread_startup(gp2y);
}
MSH_CMD_EXPORT(thread_start, thread start);

/**
 * @brief 数据接口
 */
static rt_thread_t data_interface = RT_NULL;
static void data_interface_entry(void *parameter)
{
    uint8_t str_send[200];
    uint8_t str_recv[200];
    uint8_t user[] = "SprIne";

    MX_UART5_Init();
    while(1)
    {
        // sprintf((char *)str_send, "\nHello %s! This is a sample test for the UART communication between linux and stm32.", user);
        // HAL_UART_Transmit(&huart5, (uint8_t *)str_send, strlen((const char *)str_send), 0xFFFF);

        HAL_UART_Receive(&huart5, (uint8_t *)str_recv, 2, 0xFFFF);
        rt_kprintf("\n get the message from linux: %s", str_recv);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        delay_ms(1000);
    }
}

void data_interface_start(void)
{
    data_interface = rt_thread_create("data_interface",
                                      data_interface_entry,
                                      RT_NULL,
                                      THREAD_STACK_SIZE,
                                      THREAD_PRIORITY,
                                      THREAD_TIMESLICE);
    if (data_interface != RT_NULL)
        rt_thread_startup(data_interface);
}
MSH_CMD_EXPORT(data_interface_start, data interface start);

/**
 * @brief kprintf test function
 */
static rt_thread_t kprintf_test = RT_NULL;
void kprintf_test_entry(void *parameter)
{
    while (1)
    {
        rt_kprintf("hello! %s.\n", "SprIne");
        rt_thread_delay(RT_TICK_PER_SECOND * 2);
    }
}
void kprintf_test_start(void)
{
    kprintf_test = rt_thread_create("kprintf_test",
                                    kprintf_test_entry,
                                    RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY,
                                    THREAD_TIMESLICE);
    if (kprintf_test != RT_NULL)
        rt_thread_startup(kprintf_test);
}
MSH_CMD_EXPORT(kprintf_test_start, kprintf test start);

/**
 * @brief main function
 * @return int
 */
int main(void)
{
    BSP_LED_Init();
    // thread_start();
    while (1)
    {
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        // delay_ms(500);
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        delay_ms(500);
    }
}
