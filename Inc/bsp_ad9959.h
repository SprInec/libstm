/**
 ******************************************************************************
 * @file			:bsp_ad9959.h
 * @brief			:The board support package for AD9959.
 * @version		    :0.1.1
 * @author		    :July
 * @date			:2022.07.01
 * @updata		    :2022.07.14
 ******************************************************************************
 * @pinset		:
 *
 *				F1RC:
 *							GPIOB_PIN_3 	---> P0
 *							GPIOB_PIN_4 	---> P1
 *							GPIOB_PIN_5		---> P2
 *							GPIOB_PIN_6		---> P3
 *							GPIOB_PIN_7		---> SD0
 *							GPIOC_PIN_10	---> SD1
 *							GPIOC_PIN_11	---> SD2
 *							GPIOC_PIN_9		---> SD3
 *							GPIOC_PIN_12	---> PDC/PWR
 *							GPIOA_PIN_15	---> UPDATA
 *							GPIOC_PIN_5		---> RST
 *							GPIOC_PIN_7		---> CS
 *							GPIOC_PIN_8		---> SCK
 *
 *				F4ZG:
 *							GPIOD_PIN_9 	---> P0
 *							GPIOE_PIN_15 	---> P1
 *							GPIOE_PIN_13	---> P2
 *							GPIOE_PIN_11	---> P3
 *							GPIOE_PIN_9		---> SD0
 *							GPIOE_PIN_7		---> SD1
 *							GPIOD_PIN_0		---> SD2
 *							GPIOD_PIN_1		---> SD3
 *							GPIOD_PIN_8 	---> PDC/PWR
 *							GPIOE_PIN_12	---> UPDATA
 *							GPIOE_PIN_14	---> RST
 *							GPIOE_PIN_10	---> CS
 *							GPIOE_PIN_8 	---> SCK
 *
 *******************************************************************************
 */

#ifndef __BSP_AD9959_H__
#define __BSP_AD9959_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"
#include "bsp_usart.h"

/* ==================================== */
#define AD9959_USART_DRIVE 0
/* ==================================== */

#if AD9959_USART_DRIVE

#define USE_STDIO 0U

extern uint8_t ad9959_mode[][10];
extern uint32_t ad9959_param;

#if (USE_STDIO)
#define __AD9959_AT printf("AT\r\n")                                          /* 测试指令 */
#define __AD9959_RST printf("AT+RESET\r\n")                                   /* 复位 */
#define __AD9959_GET_VERSION printf("AT+VERSION\r\n")                         /* 版本读取 */
#define __AD9959_SET_CHANNEL(n) printf("AT+CHANNEL+%d\r\n", ad9959_param = n) /* 通道控制		1/2/3/4 */
#define __AD9959_SET_REF(n) printf("AT+REF+%d\r\n", ad9959_param = n)         /* 参考晶振频率设置		10-125	MHz */
#define __AD9959_SET_MULT(n) printf("AT+MULT+%d\r\n", ad9959_param = n)       /* 倍频数设置	1-20 */
#define __AD9959_SET_MODE(n) printf("AT+MODE+%s\r\n", ad9959_mode[n])         /* 工作模式	POINT/SWEEP/FSK2/FSK4/AM */

#define __AD9959_SET_POINT_FRE(n) printf("AT+FRE+%d\r\n", ad9959_param = n) /* 点频频率输出	1-200,000,000	Hz */
#define __AD9959_SET_POINT_AMP(n) printf("AT+AMP+%d\r\n", ad9959_param = n) /* 点频幅度输出	1-1023 */
#define __AD9959_SET_POINT_PHA(n) printf("AT+PHA+%d\r\n", ad9959_param = n) /* 点频相位输出	0-16386（对应0-360°） */

#define __AD9959_SET_SWEEP_STARTFRE(n) printf("AT+STARTFRE+%d\r\n", ad9959_param = n) /* 扫频开始频率	1-200,000,000	Hz */
#define __AD9959_SET_SWEEP_ENDFRE(n) printf("AT+ENDFRE+%d\r\n", ad9959_param = n)     /* 扫频结束频率	1-200,000,000	Hz */
#define __AD9959_SET_SWEEP_STARTAMP(n) printf("AT+STARTAMP+%d\r\n", ad9959_param = n) /* 扫频开始幅度	1-1023 */
#define __AD9959_SET_SWEEP_ENDAMP(n) printf("AT+ENDAMP+%d\r\n", ad9959_param = n)     /* 扫频结束幅度	1-1023 */
#define __AD9959_SET_SWEEP_TIME(n) printf("AT+TIME+%d\r\n", ad9959_param = n)         /* 扫频间隔时间	0-9999	ms */
#define __AD9959_SET_SWEEP_STEP(n) printf("AT+STEP+%d\r\n", ad9959_param = n)         /* 扫频间隔频率	1-200,000,000	Hz */
#define __AD9959_SWEEP_ON printf("AT+SWEEP+ON\r\n", ad9959_param = n)                 /* 扫频开始 */
#define __AD9959_SWEEP_OFF printf("AT+SWEEP+OFF\r\n", ad9959_param = n)               /* 扫频结束 */

#define __AD9959_SET_2FSK_FRE1(n) printf("AT+FRE1+%d\r\n", ad9959_param = n) /* 2FSK模式频率1		1-200,000,000	Hz */
#define __AD9959_SET_2FSK_FRE2(n) printf("AT+FRE2+%d\r\n", ad9959_param = n) /* 2FSK模式频率2		1-200,000,000	Hz */

#define __AD9959_SET_4FSK_FRE1(n) printf("AT+FRE1+%d\r\n", ad9959_param = n) /* 4FSK模式频率1		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE2(n) printf("AT+FRE2+%d\r\n", ad9959_param = n) /* 4FSK模式频率2		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE3(n) printf("AT+FRE3+%d\r\n", ad9959_param = n) /* 4FSK模式频率3		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE4(n) printf("AT+FRE4+%d\r\n", ad9959_param = n) /* 4FSK模式频率4		1-200,000,000	Hz */

#define __AD9959_SET_AM_FRE(n) printf("AT+FRE+%d\r\n", ad9959_param = n)   /* AM频率	1-200,000,000	Hz */
#define __AD9959_SET_AM_DEP(n) printf("AT+DEP+%d\r\n", ad9959_param = n)   /* AM调制深度	1-100 */
#define __AD9959_SET_AM_RATE(n) printf("AT+RATE+%d\r\n", ad9959_param = n) /* AM调制速率	20-600	Hz */
#define __AD9959_AM_ON printf("AT+AM+ON\r\n", ad9959_param = n)            /* 开始AM调制 */
#define __AD9959_AM_OFF printf("AT+AM+OFF\r\n", ad9959_param = n)          /* 结束AM调制 */
#else
#define __AD9959_AT bsprif1("AT\n")                                          /* 测试指令 */
#define __AD9959_RST bsprif1("AT+RESET\n")                                   /* 复位 */
#define __AD9959_GET_VERSION bsprif1("AT+VERSION\n")                         /* 版本读取 */
#define __AD9959_SET_CHANNEL(n) bsprif1("AT+CHANNEL+%d\n", ad9959_param = n) /* 通道控制		1/2/3/4 */
#define __AD9959_SET_REF(n) bsprif1("AT+REF+%d\n", ad9959_param = n)         /* 参考晶振频率设置		10-125	MHz */
#define __AD9959_SET_MULT(n) bsprif1("AT+MULT+%d\n", ad9959_param = n)       /* 倍频数设置	1-20 */
#define __AD9959_SET_MODE(n) bsprif1("AT+MODE+%s\n", ad9959_mode[n])         /* 工作模式	POINT/SWEEP/FSK2/FSK4/AM */

#define __AD9959_SET_POINT_FRE(n) bsprif1("AT+FRE+%d\n", ad9959_param = n) /* 点频频率输出	1-200,000,000	Hz */
#define __AD9959_SET_POINT_AMP(n) bsprif1("AT+AMP+%d\n", ad9959_param = n) /* 点频幅度输出	1-1023 */
#define __AD9959_SET_POINT_PHA(n) bsprif1("AT+PHA+%d\n", ad9959_param = n) /* 点频相位输出	0-16386（对应0-360°） */

#define __AD9959_SET_SWEEP_STARTFRE(n) bsprif1("AT+STARTFRE+%d\r\n", ad9959_param = n) /* 扫频开始频率	1-200,000,000	Hz */
#define __AD9959_SET_SWEEP_ENDFRE(n) bsprif1("AT+ENDFRE+%d\r\n", ad9959_param = n)     /* 扫频结束频率	1-200,000,000	Hz */
#define __AD9959_SET_SWEEP_STARTAMP(n) bsprif1("AT+STARTAMP+%d\r\n", ad9959_param = n) /* 扫频开始幅度	1-1023 */
#define __AD9959_SET_SWEEP_ENDAMP(n) bsprif1("AT+ENDAMP+%d\r\n", ad9959_param = n)     /* 扫频结束幅度	1-1023 */
#define __AD9959_SET_SWEEP_TIME(n) bsprif1("AT+TIME+%d\r\n", ad9959_param = n)         /* 扫频间隔时间	0-9999	ms */
#define __AD9959_SET_SWEEP_STEP(n) bsprif1("AT+STEP+%d\r\n", ad9959_param = n)         /* 扫频间隔频率	1-200,000,000	Hz */
#define __AD9959_SWEEP_ON bsprif1("AT+SWEEP+ON\r\n", ad9959_param = n)                 /* 扫频开始 */
#define __AD9959_SWEEP_OFF bsprif1("AT+SWEEP+OFF\r\n", ad9959_param = n)               /* 扫频结束 */

#define __AD9959_SET_2FSK_FRE1(n) bsprif1("AT+FRE1+%d\r\n", ad9959_param = n) /* 2FSK模式频率1		1-200,000,000	Hz */
#define __AD9959_SET_2FSK_FRE2(n) bsprif1("AT+FRE2+%d\r\n", ad9959_param = n) /* 2FSK模式频率2		1-200,000,000	Hz */

#define __AD9959_SET_4FSK_FRE1(n) bsprif1("AT+FRE1+%d\r\n", ad9959_param = n) /* 4FSK模式频率1		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE2(n) bsprif1("AT+FRE2+%d\r\n", ad9959_param = n) /* 4FSK模式频率2		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE3(n) bsprif1("AT+FRE3+%d\r\n", ad9959_param = n) /* 4FSK模式频率3		1-200,000,000	Hz */
#define __AD9959_SET_4FSK_FRE4(n) bsprif1("AT+FRE4+%d\r\n", ad9959_param = n) /* 4FSK模式频率4		1-200,000,000	Hz */

#define __AD9959_SET_AM_FRE(n) bsprif1("AT+FRE+%d\r\n", ad9959_param = n)   /* AM频率	1-200,000,000	Hz */
#define __AD9959_SET_AM_DEP(n) bsprif1("AT+DEP+%d\r\n", ad9959_param = n)   /* AM调制深度	1-100 */
#define __AD9959_SET_AM_RATE(n) bsprif1("AT+RATE+%d\r\n", ad9959_param = n) /* AM调制速率	20-600	Hz */
#define __AD9959_AM_ON bsprif1("AT+AM+ON\r\n", ad9959_param = n)            /* 开始AM调制 */
#define __AD9959_AM_OFF bsprif1("AT+AM+OFF\r\n", ad9959_param = n)          /* 结束AM调制 */
#endif
#else

#ifdef __BSP_STM32F1_ENABLED
// 引脚定义
#define AD9959_PS0_Pin GPIO_PIN_3
#define AD9959_PS0_GPIO_Port GPIOB

#define AD9959_PS1_Pin GPIO_PIN_4
#define AD9959_PS1_GPIO_Port GPIOB

#define AD9959_PS2_Pin GPIO_PIN_5
#define AD9959_PS2_GPIO_Port GPIOB

#define AD9959_PS3_Pin GPIO_PIN_6
#define AD9959_PS3_GPIO_Port GPIOB

#define AD9959_SDIO0_Pin GPIO_PIN_7
#define AD9959_SDIO0_GPIO_Port GPIOB

#define AD9959_SDIO1_Pin GPIO_PIN_10
#define AD9959_SDIO1_GPIO_Port GPIOC

#define AD9959_SDIO2_Pin GPIO_PIN_11
#define AD9959_SDIO2_GPIO_Port GPIOC

#define AD9959_PWR_Pin GPIO_PIN_12
#define AD9959_PWR_GPIO_Port GPIOC

#define AD9959_UPDATE_Pin GPIO_PIN_15
#define AD9959_UPDATE_GPIO_Port GPIOA

#define AD9959_Reset_Pin GPIO_PIN_5
#define AD9959_Reset_GPIO_Port GPIOC

#define AD9959_CS_Pin GPIO_PIN_7
#define AD9959_CS_GPIO_Port GPIOC

#define AD9959_SCLK_Pin GPIO_PIN_8
#define AD9959_SCLK_GPIO_Port GPIOC

#define AD9959_SDIO3_Pin GPIO_PIN_9
#define AD9959_SDIO3_GPIO_Port GPIOC
#else /* STM32F4 */
// 引脚定义
#define AD9959_PS0_Pin GPIO_PIN_9
#define AD9959_PS0_GPIO_Port GPIOD

#define AD9959_PS1_Pin GPIO_PIN_15
#define AD9959_PS1_GPIO_Port GPIOE

#define AD9959_PS2_Pin GPIO_PIN_13
#define AD9959_PS2_GPIO_Port GPIOE

#define AD9959_PS3_Pin GPIO_PIN_11
#define AD9959_PS3_GPIO_Port GPIOE

#define AD9959_SDIO0_Pin GPIO_PIN_9
#define AD9959_SDIO0_GPIO_Port GPIOE

#define AD9959_SDIO1_Pin GPIO_PIN_7
#define AD9959_SDIO1_GPIO_Port GPIOE

#define AD9959_SDIO2_Pin GPIO_PIN_0
#define AD9959_SDIO2_GPIO_Port GPIOD

#define AD9959_PWR_Pin GPIO_PIN_8
#define AD9959_PWR_GPIO_Port GPIOD

#define AD9959_UPDATE_Pin GPIO_PIN_12
#define AD9959_UPDATE_GPIO_Port GPIOE

#define AD9959_Reset_Pin GPIO_PIN_14
#define AD9959_Reset_GPIO_Port GPIOE

#define AD9959_CS_Pin GPIO_PIN_10
#define AD9959_CS_GPIO_Port GPIOE

#define AD9959_SCLK_Pin GPIO_PIN_8
#define AD9959_SCLK_GPIO_Port GPIOE

#define AD9959_SDIO3_Pin GPIO_PIN_1
#define AD9959_SDIO3_GPIO_Port GPIOD
#endif

#define CS PEout(10)
#define SCLK PEout(8)
#define UPDATE PEout(12)

#define PS0 PDout(9)
#define PS1 PEout(15)
#define PS2 PEout(13)
#define PS3 PEout(11)

#define SDIO0 PEout(9)
#define SDIO1 PEout(7)
#define SDIO2 PDout(0)
#define SDIO3 PDout(1)

#define AD9959_PWR PDout(8)
#define Reset PEout(14)

/*SET*/
#define AD9959_PS0_SET HAL_GPIO_WritePin(AD9959_PS0_GPIO_Port, AD9959_PS0_Pin, GPIO_PIN_SET)
#define AD9959_PS1_SET HAL_GPIO_WritePin(AD9959_PS1_GPIO_Port, AD9959_PS1_Pin, GPIO_PIN_SET)
#define AD9959_PS2_SET HAL_GPIO_WritePin(AD9959_PS2_GPIO_Port, AD9959_PS2_Pin, GPIO_PIN_SET)
#define AD9959_PS3_SET HAL_GPIO_WritePin(AD9959_PS3_GPIO_Port, AD9959_PS3_Pin, GPIO_PIN_SET)
#define AD9959_SDIO0_SET HAL_GPIO_WritePin(AD9959_SDIO0_GPIO_Port, AD9959_SDIO0_Pin, GPIO_PIN_SET)
#define AD9959_SDIO1_SET HAL_GPIO_WritePin(AD9959_SDIO1_GPIO_Port, AD9959_SDIO1_Pin, GPIO_PIN_SET)
#define AD9959_SDIO2_SET HAL_GPIO_WritePin(AD9959_SDIO2_GPIO_Port, AD9959_SDIO2_Pin, GPIO_PIN_SET)

#define AD9959_PWR_SET HAL_GPIO_WritePin(AD9959_PWR_GPIO_Port, AD9959_PWR_Pin, GPIO_PIN_SET)
#define AD9959_Reset_SET HAL_GPIO_WritePin(AD9959_Reset_GPIO_Port, AD9959_Reset_Pin, GPIO_PIN_SET)
#define AD9959_UPDATE_SET HAL_GPIO_WritePin(AD9959_UPDATE_GPIO_Port, AD9959_UPDATE_Pin, GPIO_PIN_SET)
#define AD9959_CS_SET HAL_GPIO_WritePin(AD9959_CS_GPIO_Port, AD9959_CS_Pin, GPIO_PIN_SET)
#define AD9959_SCLK_SET HAL_GPIO_WritePin(AD9959_SCLK_GPIO_Port, AD9959_SCLK_Pin, GPIO_PIN_SET)
#define AD9959_SDIO3_SET HAL_GPIO_WritePin(AD9959_SDIO3_GPIO_Port, AD9959_SDIO3_Pin, GPIO_PIN_SET)

/*RESET*/
#define AD9959_PS0_RESET HAL_GPIO_WritePin(AD9959_PS0_GPIO_Port, AD9959_PS0_Pin, GPIO_PIN_RESET)
#define AD9959_PS1_RESET HAL_GPIO_WritePin(AD9959_PS1_GPIO_Port, AD9959_PS1_Pin, GPIO_PIN_RESET)
#define AD9959_PS2_RESET HAL_GPIO_WritePin(AD9959_PS2_GPIO_Port, AD9959_PS2_Pin, GPIO_PIN_RESET)
#define AD9959_PS3_RESET HAL_GPIO_WritePin(AD9959_PS3_GPIO_Port, AD9959_PS3_Pin, GPIO_PIN_RESET)
#define AD9959_SDIO0_RESET HAL_GPIO_WritePin(AD9959_SDIO0_GPIO_Port, AD9959_SDIO0_Pin, GPIO_PIN_RESET)
#define AD9959_SDIO1_RESET HAL_GPIO_WritePin(AD9959_SDIO1_GPIO_Port, AD9959_SDIO1_Pin, GPIO_PIN_RESET)
#define AD9959_SDIO2_RESET HAL_GPIO_WritePin(AD9959_SDIO2_GPIO_Port, AD9959_SDIO2_Pin, GPIO_PIN_RESET)

#define AD9959_PWR_RESET HAL_GPIO_WritePin(AD9959_PWR_GPIO_Port, AD9959_PWR_Pin, GPIO_PIN_RESET)
#define AD9959_Reset_RESET HAL_GPIO_WritePin(AD9959_Reset_GPIO_Port, AD9959_Reset_Pin, GPIO_PIN_RESET)
#define AD9959_UPDATE_RESET HAL_GPIO_WritePin(AD9959_UPDATE_GPIO_Port, AD9959_UPDATE_Pin, GPIO_PIN_RESET)
#define AD9959_CS_RESET HAL_GPIO_WritePin(AD9959_CS_GPIO_Port, AD9959_CS_Pin, GPIO_PIN_RESET)
#define AD9959_SCLK_RESET HAL_GPIO_WritePin(AD9959_SCLK_GPIO_Port, AD9959_SCLK_Pin, GPIO_PIN_RESET)
#define AD9959_SDIO3_RESET HAL_GPIO_WritePin(AD9959_SDIO3_GPIO_Port, AD9959_SDIO3_Pin, GPIO_PIN_RESET)

#define AD9959_Mode_Sweep 1    // 模式--扫频
#define AD9959_Mode_FixedFre 2 // 模式--点频
#define AD9959_Mode_FHSS 3     // 模式--跳频

extern uint8_t AD9959_Mode;           // 扫频或者点频
extern uint8_t AD9959_Wave_Show_Mode; // 跟随扫频--1  不跟随扫频--0

extern uint32_t AD9959_SweepMaxFre;     // 最大扫频频率--Hz
extern uint32_t AD9959_SweepMinFre;     // 最小扫频频率--Hz
extern uint32_t AD9959_SweepStepFre;    // 扫频步进频率--Hz
extern uint32_t AD9959_SweepTime;       // 扫频间隔时间--ms
extern uint8_t AD9959_SweepWaveFlag;    // 是否显示波形
extern uint32_t AD9959_SweepCount;      // extern到定时器中--ms
extern uint32_t AD9959_SweepCountTimes; // extern到定时器中

extern uint32_t AD9959_SweepMaxPha;  // 最大扫描相位
extern uint32_t AD9959_SweepMinPha;  // 最小扫描相位
extern uint32_t AD9959_SweepStepPha; // 扫相步进相位

extern uint32_t AD9959_FixedMaxFre;  // 最大固定输出频率--Hz
extern uint32_t AD9959_FixedMinFre;  // 最小固定输出频率--Hz
extern uint32_t AD9959_FixedStepFre; // 步进固定输出频率--Hz
extern uint32_t AD9959_FixedAmpli;   // 最大输出频率幅度--Hz
extern uint32_t AD9959_FixedNowFre;  // 此时输出频率--Hz

extern uint32_t AD9959_NowSinFre[5];
extern uint32_t AD9959_NowSinAmp[5];
extern uint32_t AD9959_NowSinPhr[5];

// AD9959寄存器地址定义
#define CSR_ADD 0x00           // CSR 通道选择寄存器
#define FR1_ADD 0x01           // FR1 功能寄存器1
#define FR2_ADD 0x02           // FR2 功能寄存器2
#define CFR_ADD 0x03           // CFR 通道功能寄存器
#define CFTW0_ADD 0x04         // CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05         // CPW0 通道相位转换字寄存器
#define ACR_ADD 0x06           // ACR 幅度控制寄存器
#define LSRR_ADD 0x07          // LSR 通道线性扫描寄存器
#define RDW_ADD 0x08           // RDW 通道线性向上扫描寄存器
#define FDW_ADD 0x09           // FDW 通道线性向下扫描寄存器

#define PROFILE_ADDR_BASE 0x0A // Profile寄存器,配置文件寄存器起始地址
// CSR[7:4]通道选择启用位
#define CH0 0x10
#define CH1 0x20
#define CH2 0x40
#define CH3 0x80
// FR1[9:8] 调制电平选择位
#define LEVEL_MOD_2 0x00   // 2电平调制 2阶调制
#define LEVEL_MOD_4 0x01   // 4电平调制	4阶调制
#define LEVEL_MOD_8 0x02   // 8电平调制	8阶调制
#define LEVEL_MOD_16 0x03  // 16电平调制	16阶调制
// CFR[23:22]  幅频相位（AFP）选择位
#define DISABLE_Mod 0x00   // 00	调制已禁用
#define ASK 0x40           // 01	振幅调制，幅移键控
#define FSK 0x80           // 10	频率调制，频移键控
#define PSK 0xc0           // 11	相位调制，相移键控
// （CFR[14]）线性扫描启用 sweep enable
#define SWEEP_ENABLE 0x40  // 1	启用
#define SWEEP_DISABLE 0x00 // 0	不启用

// 延时
void delay1(uint32_t length);
// IO口初始化
void Intserve(void);
// AD9959复位
void IntReset(void);
// AD9959更新数据
void AD9959_IO_Update(void);
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData, uint8_t temp);
void AD9959_WriteData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);
/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void AD9959_WriteFreq(uint8_t Channel, uint32_t Freq);
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void AD9959_WriteAmpl(uint8_t Channel, uint16_t Ampli);
/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void AD9959_WritePhase(uint8_t Channel, uint16_t Phase);
/*---------------------------------------
函数功能：AD9959初始化
---------------------------------------*/
void AD9959_Init(void);
void Write_CFTW0(uint32_t fre);   // 写CFTW0通道频率转换字寄存器
void Write_ACR(uint16_t Ampli);   // 写ACR通道幅度转换字寄存器
void Write_CPOW0(uint16_t Phase); // 写CPOW0通道相位转换字寄存器

void Write_LSRR(uint8_t rsrr, uint8_t fsrr); // 写LSRR线性扫描斜率寄存器
void Write_RDW(uint32_t r_delta);            // 写RDW上升增量寄存器
void Write_FDW(uint32_t f_delta);            // 写FDW下降增量寄存器

void Write_Profile_Fre(uint8_t profile, uint32_t data);   // 写Profile寄存器,频率
void Write_Profile_Ampli(uint8_t profile, uint16_t data); // 写Profile寄存器,幅度
void Write_Profile_Phase(uint8_t profile, uint16_t data); // 写Profile寄存器,相位

void AD9959_Modulation_Init(uint8_t Channel, uint8_t Modulation, uint8_t Sweep_en, uint8_t Nlevel); // 设置各个通道的调制模式。
void AD9959_SetFSK(uint8_t Channel, uint32_t *data, uint16_t Phase);                                // 设置FSK调制的参数
void AD9959_SetASK(uint8_t Channel, uint16_t *data, uint32_t fre, uint16_t Phase);                  // 设置ASK调制的参数
void AD9959_SetPSK(uint8_t Channel, uint16_t *data, uint32_t Freq);                                 // 设置PSK调制的参数

void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data, uint32_t e_data, uint32_t r_delta, uint32_t f_delta, uint8_t rsrr, uint8_t fsrr, uint16_t Ampli, uint16_t Phase); // 设置线性扫频的参数
void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli, uint16_t e_Ampli, uint32_t r_delta, uint32_t f_delta, uint8_t rsrr, uint8_t fsrr, uint32_t fre, uint16_t Phase); // 设置线性扫幅的参数
void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data, uint16_t e_data, uint16_t r_delta, uint16_t f_delta, uint8_t rsrr, uint8_t fsrr, uint32_t fre, uint16_t Ampli); // 设置线性扫相的参数

#endif

#ifdef __cplusplus
}
#endif
#endif
