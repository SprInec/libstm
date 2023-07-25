/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief User-defined function files
 * @version 0.1
 * @date ...
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _BSP_USER_H_
#define _BSP_USER_H_

#include "bsp_config.h"

#define BSP_SEND_PORT 0
#define BSP_RECV_PORT 1

#if BSP_SEND_PORT

/* 跳频时间片 */
#define TIMESLICE 300

#define DIGT_SIGNAL_1 50000
#define DIGT_SIGNAL_0 70000
#define DIGT_SIGNAL_CHANNEL 0
#define SIGNAL_BIT_TIME 20 

/* 使用数字信号发送 */
#define USING_DIGITAL 0
#define CYCLE_SEND_TIMES 3

void BSP_SendPort_Init(void);
void BSP_SendPort_CONTR(void);
void BSP_DigitalMess_Send(uint16_t code[], uint8_t len);
#endif /* BSP_SEND_PORT */

#if BSP_RECV_PORT

/* 载波输出通道 */
#define CAR_WAVE_CHANNEL 0
/* 频偏值 */
#define FREQ_OFFSET 10700000
/* 电压判断阈值 */
#define VOLTAGE_THRESHOLD_F 0.5f
#define VOLTAGE_THRESHOLD_I (VOLTAGE_THRESHOLD_F / 3.3 * 4096)

/* 扫频范围 */
#define SWEEP_RANGE 100000 /* 100k */
#define SWEEP_STEP 20000    /* 1k */

void BSP_RecvPort_Init(void);
void BSP_RecvPort_CONTR(void);
uint8_t BSP_DemoDulation_Judge(void);
#endif /* BSP_RECV_PORT */

#endif /* _BSP_USER_H_ */