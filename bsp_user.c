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

#include "bsp_user.h"
#include "tim.h"

#if BSP_SEND_PORT

/* 干扰频率 */
float intffreq = 0;
/* 当前模式 */
uint8_t nowMode = 0;
/* 改变标志 */
uint8_t changeflag = 0;
/* 数字编码 */
uint8_t digital_code[16] = {0};
/* 当前频率 */
volatile uint8_t freqPoit = 0;
/* 跳频表 */
const float freqTab[] = { 80, 81, 82, 83, 84, 85};

/**
 * @brief 发送端初始化
 */
void BSP_SendPort_Init(void)
{
    BSP_LED_Init();

    BSP_ADF4351_Init();
    ADF4351WriteFreq(freqTab[freqPoit]);

    AD9959_Init();
    AD9959_WriteAmpl(0, 1000);
    AD9959_WriteFreq(0, DIGT_SIGNAL_1);

    AD9833_Init();
    AD9833_AmpSet(15);
    AD9833_WaveSeting(10000, 0, SIN_WAVE, 0);

    HAL_TIM_Base_Start_IT(&htim7);

    bsprif2("bt1.val=1");
    __prifend2;
    bsprif2("bt0.val=0");
    __prifend2;
    for (uint8_t i = 2; i <= 7; i++)
    {
        printf("bt%d.val=0", i);
        bsprif2("bt%d.val=0", i);
        __prifend2;
    }
    bsprif2("bt2.val=1");
    __prifend2;
    bsprif2("t0.txt=\"80\"");
    __prifend2;
}

/**
 * @brief 用户串口回调
 * @param str 接收数据
 */
void BSP_UsartVar_Callback(uint8_t *str)
{
    /* 获取头部信息 */
    uint8_t head = *(str + 0);
    switch (head)
    {
    case /* MODE -> Auto */ 'A':
        nowMode = 0;
        break;
    case /* MODE -> Handle */ 'B':
        nowMode = 1;
        break;
    case /* FREQ -> 80MHz */ 'C':
        nowMode = 1;
        freqPoit = 0;
#if USING_DIGITAL
        BSP_FloatTo16BCD(80, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
        break;
    case /* FREQ -> 81MHz */ 'D':
        nowMode = 1;
        freqPoit = 1;
#if USING_DIGITAL
        BSP_FloatTo16BCD(81, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
        break;
    case /* FREQ -> 82MHz */ 'E':
        nowMode = 1;
        freqPoit = 2;
#if USING_DIGITAL
        BSP_FloatTo16BCD(82, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
        break;
    case /* FREQ -> 83MHz */ 'F':
        nowMode = 1;
        freqPoit = 3;
#if USING_DIGITAL
        BSP_FloatTo16BCD(83, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
        break;
    case /* FREQ -> 84MHz */ 'G':
        nowMode = 1;
        freqPoit = 4;
#if USING_DIGITAL
        BSP_FloatTo16BCD(84, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
        break;
    case /* FREQ -> 85MHz */ 'H':
        nowMode = 1;
        freqPoit = 5;
#if USING_DIGITAL
        BSP_FloatTo16BCD(85, digital_code);
        for (uint8_t i = 0; i < CYCLE_SEND_TIMES, i++)
        {
            BSP_DigitalMess_Send(digital_code, 16);
        }
#endif
        ADF4351WriteFreq(freqTab[freqPoit]);
    default:
        break;
    }
}

/**
 * @brief 定时器中断回调
 * @param htim 
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim7.Instance)
    {
        if (freqPoit++ >= 5)
        {
            freqPoit = 0;
        }
        __BSP_LED2_Toggle();
        changeflag = 1;
    }
}

/**
 * @brief 干扰检测
 */
void BSP_InterfDetect(void)
{
    
}

/**
 * @brief 跳频控制
 */
void BSP_FHSS_CONTR(void)
{
    /* 自动调频 */
    if ((nowMode == 0) && (changeflag == 1))
    {
        ADF4351WriteFreq(freqTab[freqPoit]);
        bsprif2("t0.txt=\"%.0f\"", freqTab[freqPoit]);
        __prifend2;
        for (uint8_t i = 2; i <= 7; i++)
        {
            printf("bt%d.val=0", i);
            bsprif2("bt%d.val=0", i);
            __prifend2;
        }
        bsprif2("bt%d.val=1", freqPoit + 2);
        __prifend2;
        changeflag = 0;
    }
}

/**
 * @brief 数字信号发送 
 */
void BSP_DigitalMess_Send(uint16_t code[], uint8_t len)
{
    /* Digital signal BEGIN bit */
    AD9959_WriteFreq(DIGT_SIGNAL_CHANNEL, DIGT_SIGNAL_0);
    delay_us(SIGNAL_BIT_TIME);

    /* Send diginal signal*/
    for (uint8_t i = 0; i < len; i++)
    {
        if (!code[i])
        {
            AD9959_WriteFreq(DIGT_SIGNAL_CHANNEL, DIGT_SIGNAL_0);
            delay_us(SIGNAL_BIT_TIME);
        }
        else
        {
            AD9959_WriteFreq(DIGT_SIGNAL_CHANNEL, DIGT_SIGNAL_1);
            delay_us(SIGNAL_BIT_TIME);
        }
    }

    /* Digital signal END bit */
    AD9959_WriteFreq(DIGT_SIGNAL_CHANNEL, DIGT_SIGNAL_0);
    delay_us(SIGNAL_BIT_TIME);

    /* Set low freqence state defaultly */
    AD9959_WriteFreq(DIGT_SIGNAL_CHANNEL, DIGT_SIGNAL_1);
}

/**
 * @brief 发送端控制函数
 */
void BSP_SendPort_CONTR(void)
{
    BSP_UsartVar_Conduct();
    // BSP_FHSS_CONTR();
}
#endif /* BSP_SEND_PORT */

#if BSP_RECV_PORT
void BSP_RecvPort_Init(void)
{
    BSP_LED_Init();
    BSP_ADF4351_Init();
    HAL_TIM_Base_Start_IT(&htim7);
}

void BSP_FHSS_Judge(void)
{

}

void BSP_RecvPort_CONTR(void)
{

}
#endif /* BSP_RECV_PORT */