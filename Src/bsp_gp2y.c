/**
 * @file bsp_gp2y.c
 * @author fire
 * @brief PM2.5/粉尘检测
 * @version 0.1
 * @date 2023.08.22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_gp2y.h"

static uint16_t buffer[MAX_NUM]; /*缓冲数组全局变量*/
ADC_HandleTypeDef ADC_Handle;

/**
 * @brief  数组缓冲ADC数值
 * @param  valve电压编码值
 * @retval 无
 */
void GP2Y_GetDate(uint16_t valve)
{
  static uint8_t k = 0;

  buffer[k] = valve;
  k++;
  if (k == MAX_NUM)
  {
    k = 0;
  }
}

/**
 * @brief  数组内缓冲ADC电压采集数据
 * @param  无
 * @retval 无
 */
void Collect_data(void)
{
  uint8_t i;

  for (i = 0; i < 2; i++)
  {
    GP2Y_ILED_SET(ILED_HIGH);
    /* 等待模块输出电压稳定 */
    delay_us(280);

    /* 开始adc转换，软件触发 */
    HAL_ADC_Start(&ADC_Handle);

    while (__HAL_ADC_GET_FLAG(&ADC_Handle, ADC_FLAG_EOC) != SET);

    /* 数组缓冲ADC转化数值*/
    GP2Y_GetDate(HAL_ADC_GetValue(&ADC_Handle));

    /* 清空ADC悬起标志位*/
    __HAL_ADC_CLEAR_FLAG(&ADC_Handle, ADC_FLAG_EOC);

    /* 关闭ILED灯 */
    GP2Y_ILED_SET(ILED_LOW);

    HAL_Delay(5);
  }
}

/**
 * @brief  电压值转化为灰尘浓度值,单位μg/m3
 * @param  dens  灰尘浓度值
 * @retval ADC_ConvertFlag浓度转化标志位
 */
double GP2Y_Convert(void)
{
  uint8_t i;
  uint16_t sum = 0;
  double voltage, dens;

  for (i = 0; i < MAX_NUM; i++)
  {
    sum += buffer[i];
  }
  voltage = (sum / 10.0) * (3300 / 4096.0) * 11; /* 实际平均电压值 */

  if (voltage <= NO_DUST_VOLTAGE)
  {
    return 0;
  }
  else
  {
    dens = (voltage - NO_DUST_VOLTAGE) * RATIO; /* 电压值换算成灰尘浓度值 */
    return dens;
  }
}

double GP2Y_GetDens(void)
{
  double dens;
  /*将采集到的AD转化数值缓存到数组内*/
  Collect_data();
  dens = GP2Y_Convert();
  return dens;
}

/**
 * @brief  ADC引脚配置函数
 * @param  无
 * @retval 无
 */
static void ADC_GPIO_Mode_Config(void)
{
  /* 定义一个GPIO_InitTypeDef类型的结构体 */
  GPIO_InitTypeDef GPIO_InitStruct;
  /* 使能ADC引脚的时钟 */
  GP2Y_ADC_GPIO_CLK_ENABLE();
  /* 使能ILED时钟 */
  GP2Y_ILED_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GP2Y_ADC_PIN;
  /* 配置为模拟输入，不需要上拉电阻 */
  HAL_GPIO_Init(GP2Y_ADC_GPIO_PORT, &GPIO_InitStruct);

  /*选择要控制的GPIO引脚*/
  GPIO_InitStruct.Pin = GP2Y_ILED_PIN;
  /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /*设置引脚速率为高速 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /*初始化 ILED IO*/
  HAL_GPIO_Init(GP2Y_ILED_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GP2Y_ILED_PORT, GP2Y_ILED_PIN, (GPIO_PinState)0);
}

/**
 * @brief  ADC工作模式配置函数
 * @param  无
 * @retval 无
 */
static void ADC_Mode_Config(void)
{
  ADC_ChannelConfTypeDef ADC_Config;

  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
  /*            配置ADC3时钟源             */
  /*    HSE Frequency(Hz)    = 25000000   */
  /*         PLL_M                = 5     */
  /*         PLL_N                = 160   */
  /*         PLL_P                = 25    */
  /*         PLL_Q                = 2     */
  /*         PLL_R                = 2     */
  /*     ADC_ker_clk         = 32000000   */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  RCC_PeriphClkInit.PLL2.PLL2FRACN = 0;
  RCC_PeriphClkInit.PLL2.PLL2M = 5;
  RCC_PeriphClkInit.PLL2.PLL2N = 160;
  RCC_PeriphClkInit.PLL2.PLL2P = 25;
  RCC_PeriphClkInit.PLL2.PLL2Q = 2;
  RCC_PeriphClkInit.PLL2.PLL2R = 2;
  RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  RCC_PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* 使能ADC时钟 */
  GP2Y_ADC_CLK_ENABLE();
  ADC_Handle.Instance = GP2Y_ADC;
  // 使能Boost模式
  // ADC_Handle.Init.BoostMode = ENABLE;
  // ADC时钟1分频
  ADC_Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  // 使能连续转换模式
  ADC_Handle.Init.ContinuousConvMode = ENABLE;
  // 数据存放在数据寄存器中
  ADC_Handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  // 关闭不连续转换模式
  ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
  // 单次转换
  ADC_Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  // 软件触发
  ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  // 关闭低功耗自动等待
  ADC_Handle.Init.LowPowerAutoWait = DISABLE;
  // 数据溢出时，覆盖写入
  ADC_Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  // 不使能过采样模式
  ADC_Handle.Init.OversamplingMode = DISABLE;
  // 分辨率为：16bit
  ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
  // 不使能多通道扫描
  ADC_Handle.Init.ScanConvMode = DISABLE;
  // 初始化 ADC
  HAL_ADC_Init(&ADC_Handle);

  // 使用通道6
  ADC_Config.Channel = GP2Y_ADC_CHANNEL;
  // 转换顺序为1
  ADC_Config.Rank = ADC_REGULAR_RANK_1;
  // 采样周期为64.5个周期
  ADC_Config.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
  // 不使用差分输入的功能
  ADC_Config.SingleDiff = ADC_SINGLE_ENDED;
  // 配置ADC通道
  HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
  // 使能ADC1、2
  ADC_Enable(&ADC_Handle);
}

/**
 * @brief  ADC初始化函数
 * @param  无
 * @retval 无
 */
void BSP_GP2Y_Init(void)
{

  ADC_GPIO_Mode_Config();
  ADC_Mode_Config();
}
