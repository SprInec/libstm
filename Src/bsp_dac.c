/*
 ******************************************************************************
 * @file		:bsp_dac.c
 * @brief		:The board support package for dac.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27
 ******************************************************************************
 */

#ifdef __BSP_DAC_ENABLED
#include "bsp_dac.h"
#include "bsp_dac_code.h"
 
#if		LisaruImage
uint16_t lisaru_counts = 0;
#endif

/* DAC软件触发 */
void bsp_DAC_TriggerNONE(void)
{
	DAC_ChannelConfTypeDef sConfig = {0};
	hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
}
/* DAC定时器触发 */
/* TIM6/TIM7 */
void bsp_DAC_TriggerTim(void)
{
	DAC_ChannelConfTypeDef sConfig = {0};
	hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
	sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif