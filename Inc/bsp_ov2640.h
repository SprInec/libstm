/**
 * @file bsp_ov2640.h
 * @author July (Email: JulyCub@163.com)
 * @brief OV2640 Driver
 * @version 0.1
 * @date 2023.05.24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _BSP_OV2640_H
#define _BSP_OV2640_H

#include "bsp_config.h"

extern __IO uint32_t jpeg_head;
extern __IO uint32_t headok;
extern __IO uint32_t jpeg_len;
extern uint8_t *jpeg_file;

extern DMA_HandleTypeDef hdma_dcmi;

typedef enum ovxmode
{
    OVRGB565 = 0,
    OVJPEG,
} OVx_MODETypeDef;

/* 补光灯 */
#define OV2640_FILL_LIGHT 0
#define OV2640_LED_PIN GPIO_PIN_5

#if MCU_SELECTION <= 2
#define OV2640_PWDN_PIN GPIO_PIN_9
#define OV2640_RST_PIN GPIO_PIN_15

#define OV2640_PWDN  	        PGout(9)		//POWER DOWN控制信号
#define OV2640_RST  	        PGout(15)		//复位控制信号 
#define OV2640_LED_light  	    PFout(8)        //补光LED灯控制引脚
#else
#define OV2640_PWDN_PIN GPIO_PIN_7
#define OV2640_RST_PIN GPIO_PIN_4

#define OV2640_PWDN(__STATE__) HAL_GPIO_WritePin(GPIOA, OV2640_PWDN_PIN, __STATE__) // POWER DOWN控制信号
#define OV2640_RST(__STATE__) HAL_GPIO_WritePin(GPIOC, OV2640_RST_PIN, __STATE__)   // 复位控制信号
#define OV2640_LED_light(__STATE__) HAL_GPIO_WritePin(GPIOF, OV2640_LED_PIN, __STATE__) // 补光LED灯控制引脚
#endif

#define OV2640_MID				0X7FA2
#define OV2640_PID1				0X2642
#define OV2640_PID2				0X2641 

/* 当选择DSP地址(0XFF=0X00)时,OV2640的DSP寄存器地址映射表 */
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE1       0x51
#define OV2640_DSP_VSIZE1       0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF 

/* 当选择传感器地址(0XFF=0X01)时,OV2640的DSP寄存器地址映射表 */
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62

u8 BSP_OV2640_Init(void);
void OV2640_YUV422JPEG_Mode(void);
void OV2640_RGB565JPEG_Mode(void);
void OV2640_RGB565_Mode(void);
void OV2640_Auto_Exposure(u8 level);
void OV2640_Light_Mode(u8 mode);
void OV2640_Color_Saturation(u8 sat);
void OV2640_Brightness(u8 bright);
void OV2640_Contrast(u8 contrast);
void OV2640_Special_Effects(u8 eft);
void OV2640_Color_Bar(u8 sw);
void OV2640_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
u8 OV2640_OutSize_Set(u16 width,u16 height);
u8 OV2640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height);
u8 OV2640_ImageSize_Set(u16 width,u16 height);

uint8_t BSP_OV2640_JPEGToUART(void);
uint8_t BSP_OV2640_RGB565ToLCD(void);
void BSP_OV2640_Controller(void);

#endif





















