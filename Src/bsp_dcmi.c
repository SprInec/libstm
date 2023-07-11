/**
 * @file bsp_dcmi.c
 * @author July (Email: JulyCub@163.com)
 * @brief BSP DCMI Driver
 * @version 0.1
 * @date 2023.05.24
 *
 * @copyright Copyright (c) 2023
 *
 * @attention
 * 			DCMI GPIO Configuration
 *			PE5     ------> DCMI_D6
 *			PE6     ------> DCMI_D7
 *			PA4     ------> DCMI_HSYNC
 *			PA6     ------> DCMI_PIXCLK
 *			PC6     ------> DCMI_D0
 *			PC7     ------> DCMI_D1
 *			PC8     ------> DCMI_D2
 *			PC9     ------> DCMI_D3
 *			PC11	------> DCMI_D4
 *			PB6     ------> DCMI_D5
 *			PB7     ------> DCMI_VSYNC
 */

#include "dcmi.h"
#include "gpio.h"
#include "dma.h"
#include "bsp_dcmi.h"
#include "bsp_lcd.h"
#include "bsp_led.h"
#include "bsp_ov2640.h"

#ifdef __BSP_STM32H7_ENABLED
extern DMA_HandleTypeDef hdma_dcmi_pssi;
#else
extern DMA_HandleTypeDef hdma_dcmi;
#endif
/* OV2640 模式 */
OVx_MODETypeDef OV2640_MODE = OVJPEG;
/* 帧率 */
uint8_t ov_frame = 0;
/* JPEG 缓存 */
uint32_t jpeg_buf[JPEG_BUFFER_SIZE] __attribute__((aligned(4)));
/* buf中的JPEG有效数据长度 */
volatile uint32_t jpeg_data_len = 0;
/* JPEG数据采集完成标志 */
volatile uint8_t jpeg_data_state = 0;
/* JPEG尺寸支持列表 */
const uint16_t jpeg_img_size_tbl[][2] = {
	176, 144,	// QCIF
	160, 120,	// QQVGA
	352, 288,	// CIF
	320, 240,	// QVGA
	640, 480,	// VGA
	800, 600,	// SVGA
	1024, 768,	// XGA
	1280, 1024, // SXGA
	1600, 1200, // UXGA
};
/* 7种特效 */
const uint8_t *EFFECTS_TBL[7] = {(uint8_t *)"Normal", (uint8_t *)"Negative", (uint8_t *)"B&W",
								 (uint8_t *)"Redish", (uint8_t *)"Greenish", (uint8_t *)"Bluish",
								 (uint8_t *)"Antique"};
/* JPEG图片 9种尺寸 */
const uint8_t *JPEG_SIZE_TBL[9] = {(uint8_t *)"QCIF", (uint8_t *)"QQVGA", (uint8_t *)"CIF",
								   (uint8_t *)"QVGA", (uint8_t *)"VGA", (uint8_t *)"SVGA",
								   (uint8_t *)"XGA", (uint8_t *)"SXGA", (uint8_t *)"UXGA"};

/**
 * @brief 初始化 DCMI GPIO 口
 */
void BSP_DCMI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief 开启DCMI
 *
 * @param DCMI_Mode DCMI 捕获模式
 * @param pData 数据流向地址
 * @param Length 数据长度
 */
void BSP_DCMI_Start(uint32_t DCMI_Mode, uint32_t pData, uint32_t Length)
{
#if DCMI_USE_LCD
	if (OV2640_MODE == RGB565)
	{
		LCD_SetCursor(0, 0);
		LCD_WriteRAM_Prepare();
	}
#endif
	HAL_DCMI_Start_DMA(&hdcmi, DCMI_Mode, pData, Length);
	__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
	__HAL_DCMI_DISABLE_IT(&hdcmi, DCMI_IT_VSYNC | DCMI_IT_LINE | DCMI_IT_OVF);
}

/**
 * @brief 停止DCMI
 */
void BSP_DCMI_Stop(void)
{
	hdcmi.Instance->CR &= !DCMI_CR_CAPTURE;
	while (hdcmi.Instance->CR & 0x01);
	HAL_DCMI_Stop(&hdcmi);
}

/**
 * @brief DCMI 帧事件回调函数
 * @param hdcmi DCMI句柄
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	/* JPEG 模式 */
	if (OV2640_MODE == OVJPEG)
	{
		if (jpeg_data_state == 0)
		{
#ifdef __BSP_STM32H7_ENABLED
			/* 停止DMA数据流传输 */
			__HAL_DMA_DISABLE(&hdma_dcmi_pssi);
			/* 获取当前帧数据长度 */
			jpeg_data_len = JPEG_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_dcmi_pssi);
			/* 重新给DMA传输数据量赋值 */
			// __HAL_DMA_SET_COUNTER(&hdma_dcmi, JPEG_BUFFER_SIZE);
			/* 开启DMA数据流传输 */
			__HAL_DMA_ENABLE(&hdma_dcmi_pssi);
#else
			/* 停止DMA数据流传输 */
			__HAL_DMA_DISABLE(&hdma_dcmi);
			/* 获取当前帧数据长度 */
			jpeg_data_len = JPEG_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_dcmi);
			/* 重新给DMA传输数据量赋值 */
			// __HAL_DMA_SET_COUNTER(&hdma_dcmi, JPEG_BUFFER_SIZE);
			/* 开启DMA数据流传输 */
			__HAL_DMA_ENABLE(&hdma_dcmi);
#endif /* !__BSP_STM32H7_ENABLED */
			/* JPEG传输状态置0 */
			jpeg_data_state = 1;
		}
	}
	/* RGB565 模式 */
	else if (OV2640_MODE == OVRGB565)
	{
#if DCMI_USE_LCD
		LCD_SetCursor(0, 0);
		LCD_WriteRAM_Prepare(); // 开始写入GRAM
		// !必要延时
		delay_ms(10);
#endif /* !DCMI_USE_LCD */
	}
}
