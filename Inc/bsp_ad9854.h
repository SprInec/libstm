/**
 * @file bsp_ad9854.h
 * @author July (Email: JulyCub@163.com)
 * @brief AD9854 Driver
 * @version 0.1
 * @date 2023.07.24
 * @pinset
 *          --
 *          MRESET      - PE6;
 *          UD/ICLK     - PC13;
 *          RSCLK/WR    - PE3;
 *          RD          - PE2;
 *          OSK         - PE4;
 *          FSK	  	    - PE5;
 *          D0		    - PF0;
 *          D1		    - PF1;
 *          D2		    - PF2;
 *          D3		    - PF3;
 *          D4		    - PF4;
 *          D5		    - PF5;
 *          D6		    - PF6;
 *          D7		    - PF7;
 *          A0		    - PF8;
 *          A1		    - PF9;
 *          A2		    - PF10;
 *          A3		    - PF11;
 *          A4		    - PF12;
 *          A5		    - PF13;
 *          VDD         逻辑电源(3.3V)
 *          VSS         GND(0V)
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __AD9854_H
#define __AD9854_H
#include "bsp_config.h"
#include "bsp_sys.h"

#define AD9854_RST PEout(6)           // AD9854复位端口
#define AD9854_UDCLK PCout(13)        // AD9854更新时钟
#define AD9854_WR PEout(3)            // AD9854写使能，低有效
#define AD9854_RD PEout(2)            // AD9854读使能，低有效
#define AD9854_OSK PEout(4)           // AD9854 OSK控制端
#define AD9854_FSK_BPSK_HOLD PEout(5) // AD9854 FSK,BPSK,HOLD控制脚，即AD9854芯片29脚

#define AD9854_DataBus GPIOF->BSRR
#define AD9854_AdrBus GPIOF->BSRR

#define AUTO 1   // 自动扫频
#define MANUAL 0 // FSK控制脚控制扫频

#ifndef uint
#define uint unsigned int
#endif
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif

void AD9854_IO_Init(void);              // AD9854需要用到的IO口初始化
void AD9854_WR_Byte(u32 addr, u32 dat); // AD9854并行口写数据
void Freq_convert(long Freq);           // 正弦信号频率数据转换
void Freq_double_convert(double Freq);  // 正弦信号频率数据转换,入口参数为double，可使信号的频率更精确

void AD9854_InitSingle(void);                        // AD9854点频模式初始化
void AD9854_SetSine(ulong Freq, uint Shape);         // AD9854正弦波产生程序,频率为整数
void AD9854_SetSine_double(double Freq, uint Shape); // AD9854正弦波产生程序,频率可为小数

void AD9854_InitFSK(void);                    // AD9854的FSK模式初始化
void AD9854_SetFSK(ulong Freq1, ulong Freq2); // AD9854的FSK参数设置

void AD9854_InitRFSK(uchar autoSweepEn);                                                 // AD9854的RFSK模式初始化 线性调频模式即扫频
void AD9854_SetRFSK(ulong Freq_Low, ulong Freq_High, ulong Freq_Up_Down, ulong FreRate); // AD9854的RFSK扫频参数设置

void AD9854_InitChirp(void);                                                     // AD9854的Chirp模式初始化
void AD9854_SetChirp(ulong Freq, uint Shape, ulong Freq_Up_Down, ulong FreRate); // AD9854的Chirp扫频参数设置

void AD9854_InitBPSK(void);                                            // AD9854的BPSK模式初始化
void AD9854_SetBPSK(ulong Freq, uint Shape, uint Phase1, uint Phase2); // AD9854的BPSK参数设置

void AD9854_InitOSK(void);                       // AD9854的OSK模式初始化
void AD9854_SetOSK(ulong Freq, uchar RateShape); // AD9854的OSK参数设置

#endif
