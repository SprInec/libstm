/**
 * @file bsp_dcmi.h
 * @author July (Email: JulyCub@163.com)
 * @brief DCMI Driver
 * @version 0.1
 * @date 2023.05.24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _BSP_DCMI_H
#define _BSP_DCMI_H
#include "bsp_sys.h"

#define DCMI_USE_LCD 0
#define JPEG_BUFFER_SIZE (31 * 1024)

extern uint8_t ov_frame;
extern uint32_t jpeg_buf[JPEG_BUFFER_SIZE];
extern volatile uint32_t jpeg_data_len;
extern volatile uint8_t jpeg_data_state;
extern const uint16_t jpeg_img_size_tbl[][2];
extern const uint8_t *EFFECTS_TBL[7];
extern const uint8_t *JPEG_SIZE_TBL[9];

void BSP_DCMI_GPIO_Init(void);
void BSP_DCMI_Start(uint32_t DCMI_Mode, uint32_t pData, uint32_t Length);
void BSP_DCMI_Stop(void);

#endif





















