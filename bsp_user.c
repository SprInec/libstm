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
#include "arm_math.h"

#if BSP_SEND_PORT

/* 干扰频率 */
float intffreq = 0;
/* 当前模式 */
volatile uint8_t nowMode = 1;
/* 改变标志 */
uint8_t changeflag = 0;
/* 数字编码 */
uint8_t digital_code[16] = {0};
/* 当前频率 */
volatile uint8_t freqPoit = 0;
/* 跳频表 */
const float freqTab[] = { 80.7, 81.7, 82.7, 83.7, 84.7, 85.7};
/* 自定义频率 */
float diyfreq = 0;

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

    bsprif3("bt1.val=0");
    __prifend3;
    bsprif3("bt0.val=1");
    __prifend3;
    for (uint8_t i = 2; i <= 7; i++)
    {
        bsprif3("bt%d.val=0", i);
        __prifend3;
    }
    bsprif3("bt2.val=1");
    __prifend3;
    bsprif3("t0.txt=\"80\"");
    __prifend3;
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
    case 'J':
        nowMode = 2;
        break;
    case /* MODE -> Handle */ 'B':
        nowMode = 1;
        break;
    case /* MODE -> DIY */'I':
        nowMode = 2;
        sscanf((char *)str, "I%f", &diyfreq);
        BSP_ADF4351_Init();
        ADF4351WriteFreq(diyfreq);
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
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
        BSP_ADF4351_Init();
        ADF4351WriteFreq(freqTab[freqPoit]);
        bsprif3("t0.txt=\"%d\"", bsp_downint(freqTab[freqPoit]));
        __prifend3;
        for (uint8_t i = 2; i <= 7; i++)
        {
            bsprif3("bt%d.val=0", i);
            __prifend3;
            if (nowMode != 0)
            {
                break;
            }
        }
        bsprif3("bt%d.val=1", freqPoit + 2);
        __prifend3;
        changeflag = 0;
        delay_ms(200);
    }
}

#if USING_DIGITAL
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
#endif

/**
 * @brief 发送端控制函数
 */
void BSP_SendPort_CONTR(void)
{
    BSP_UsartVar_Conduct();
    BSP_FHSS_CONTR();
}
#endif /* BSP_SEND_PORT */

#if BSP_RECV_PORT

#include "adc.h"

/* 跳频表 */
const uint32_t freqTab[] = {80700000 + FREQ_OFFSET,
                            81700000 + FREQ_OFFSET,
                            82700000 + FREQ_OFFSET,
                            83700000 + FREQ_OFFSET,
                            84700000 + FREQ_OFFSET,
                            85700000 + FREQ_OFFSET};
/* 当前表位 */
uint8_t tabpoint = 0;
/* 当前频率 */
uint32_t nowFreq = 80000000 + FREQ_OFFSET;
/* 连接状态 */
uint8_t sync_state = 0;
/* ADC压值 */ 
uint16_t adc_val = 0;
/* 接收信息 */
uint16_t mess_freq = 0;

void BSP_RecvPort_Init(void)
{
    /* LED init */
    BSP_LED_Init();

    /* AD9959 init */
    AD9959_Init();
    AD9959_WriteAmpl(CAR_WAVE_CHANNEL, 1023);
    AD9959_WriteFreq(CAR_WAVE_CHANNEL, nowFreq);

    bsprif3("t0.txt=\"80\"");
    __prifend3;
    bsprif3("t1.txt=\"DISCONNECT\"");
    __prifend3;
    bsprif3("t2.txt=\"SYNCING-%.2f\"", freqTab[tabpoint] / 1000000);
    __prifend3;

    HAL_TIM_Base_Start(&htim8);
    HAL_TIM_Base_Start_IT(&htim6);
}

/**
 * @brief FHSS同步
 */
void BSP_FHSS_Sync(void)
{

}

/**
 * @brief 定时器中断回调
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance)
    {
        mess_freq = __HAL_TIM_GET_COUNTER(&htim8);
        __HAL_TIM_SET_COUNTER(&htim8, 0);
        __BSP_LED2_Toggle();
    }
}

/**
 * @brief 解调判断
 * @return uint8_t 1->解调失败 0->解调成功
 */
uint8_t BSP_DemoDulation_Judge(void)
{
    HAL_ADC_Start(&hadc1);
    adc_val = HAL_ADC_GetValue(&hadc1);
    __prifend3;
    if (adc_val < VOLTAGE_THRESHOLD_I){
        bsprif3("t0.txt=\"%.2f\"", (nowFreq - FREQ_OFFSET) / 1000000.0);
        __prifend3;
        bsprif3("t1.txt=\"CONNECT\"");
        __prifend3;
        bsprif3("t2.txt=\"- -\"");
        __prifend3;
        sync_state = 1;
        return 0;
    }
    else{
        bsprif3("t0.txt=\"%.2f\"", (nowFreq - FREQ_OFFSET) / 1000000.0);
        __prifend3;
        bsprif3("t1.txt=\"DISCONNECT\"");
        __prifend3;
        bsprif3("t2.txt=\"SYNCING-%.2f\"", freqTab[tabpoint] / 1000000);
        __prifend3;
        sync_state = 0;
        return 1;
    }
}

/**
 * @brief 频率扫描
 * @param centerfreq 中心频率
 * @param sweepwidth 扫频范围
 * @param sweepstep 扫频步进
 * @return uint8_t 0->扫频成功 1->扫频失败
 */
uint8_t BSP_Freq_Sweep(uint32_t centerfreq, uint32_t sweepwidth, float sweepstep)
{
    uint16_t i = 0;
    for (i = 0; (i * sweepstep) < (2 * sweepwidth); i++)
    {
        nowFreq = (centerfreq - sweepwidth) + (i * sweepstep);
        AD9959_WriteFreq(CAR_WAVE_CHANNEL, nowFreq);
        delay_ms(10);
        if (!BSP_DemoDulation_Judge()){
            bsprif3("t0.txt=\"%.2f\"", (nowFreq - FREQ_OFFSET) / 1000000.0);
            __prifend3;
            bsprif3("t1.txt=\"CONNECT\"");
            __prifend3;
            bsprif3("t2.txt=\"- -\"");
            __prifend3;
            return 0;
        }
        bsprif3("t2.txt=\"SYNCING-%.2f\"", freqTab[tabpoint] / 1000000);
        __prifend3;
    }
    return 1;
}

/**
 * @brief 跳频同步
 */
void BSP_Tab_Sync(void)
{
    uint8_t count = 0;
    while (BSP_Freq_Sweep(freqTab[tabpoint], SWEEP_RANGE, SWEEP_STEP))
    {
        tabpoint++;
        if (tabpoint >= 6){
            tabpoint = 0;
        }
        count++;
        if (count >= 6){
            break;
        }
    }
}


/**
 * @brief 接收端控制函数
 */
void BSP_RecvPort_CONTR(void)
{
    if (!sync_state)
    {
        bsprif3("t0.txt=\"%.2f\"", (nowFreq - FREQ_OFFSET) / 1000000.0);
        __prifend3;
        bsprif3("t1.txt=\"DISCONNECT\"");
        __prifend3;
        bsprif3("t2.txt=\"SYNCING-%.2f\"", freqTab[tabpoint] / 1000000);
        __prifend3;
    }
    if (BSP_DemoDulation_Judge())
    {
        BSP_Tab_Sync();
    }
    bsprif3("t3.txt=\"%d\"", mess_freq);
    printf("freq = %d\n", mess_freq);
    __prifend3;
}
#endif /* BSP_RECV_PORT */