/**
 * @file bsp_decode.c
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for decode.
 * @version 0.1
 * @date 2022.06.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stdio.h"
#include "bsp_decode.h"

/**
 * @brief 16位数组转无符号浮点数
 * @param arry 源数据，定长为16
 * @attention 高8位整数,低8位小数
 * @return float
 */
float BSP_Arry16ToUFloat(uint16_t arry[])
{
    uint16_t temp = 0;
    float result = 0;

    for (uint8_t i = 0; i < 16; i++)
    {
        temp <<= 1;
        temp += arry[i];
        switch (i)
        {
        case 7:
            result += temp;
            temp = 0;
            break;
        case 15:
            result += temp % 100 / 100.0;
            temp = 0;
            break;
        default:
            break;
        }
    }
    return result;
}

/**
 * @brief 24位数组转无符号浮点数
 * @param arry 源数据，定长为24
 * @attention 高8位为2位整数,低16位位4位小数
 * @return 转换结果
 */
float BSP_Arry24ToUFloat(uint8_t arry[])
{
    uint16_t temp = 0;
    float result = 0;

    for (uint8_t i = 0; i < 24; i++)
    {
        temp <<= 1;
        temp += arry[i];
        switch (i)
        {
        case 7:
            result += temp;
            temp = 0;
            break;
        case 15:
            result += temp % 100 / 100.0;
            temp = 0;
            break;
        case 23:
            result += temp % 100 / 10000.0;
            break;
        default:
            break;
        }
    }
    return result;
}

/**
 * @brief 归一化
 *
 * @param data 采样数据
 * @param result 归一化结果
 * @param len 采样数据长度
 * @param threshold 阈值
 */
void BSP_DataNormalization(uint16_t data[], uint16_t result[], uint16_t len, uint16_t threshold)
{
    for (uint16_t i = 0; i < len; i++)
    {
        if (data[i] <= threshold)
            result[i] = 0;
        else
            result[i] = 1;
    }
}

/**
 * @brief 检波算法
 *
 * @param data 源数据
 * @param result 处理结果
 * @param data_len 数据长度
 * @param bitlen 位宽
 */
void BSP_DigitalDetector(uint16_t *data, uint16_t *result, uint16_t data_len, uint16_t bitlen)
{
    uint8_t lock = 0;
    uint16_t Zeros = 0;

    for (uint16_t i = 0; i < data_len - 1; i++)
    {
        if (*(data + i + 1) - *(data + i) == -1) /* 下降沿检测 */
        {
            *(result + i) = 1;
            *(result + i + 1) = 1;

            /* Zeros自锁 */
            lock = 1;
            while (lock)
            {
                i++; /* 数据地址递增 */
                /* 上升沿检测 */
                if (*(data + i))
                    lock = 0; /* Zeros解锁 */
                Zeros++;      /* 0值累加 */
            }
            /* 位0 */
            if (Zeros >= bitlen)
            {
                for (uint16_t j = 1; j < Zeros; j++)
                {
                    *(result + (i - j)) = 0;
                }
            }
            /* 位1 */
            else
            {
                for (uint16_t j = 0; j < Zeros; j++)
                {
                    *(result + (i - j)) = 1;
                }
            }
            Zeros = 0;
        }
        else
        {
            *(result + i) = 1;
        }
    }
}

/**
 * @brief 帧数据提取
 *
 * @version 1.0
 * @param data 源数据
 * @param result 提取结果
 * @param data_len 源数据长度
 * @param bitlen 位宽
 * @param headtaillen 帧头/尾宽度
 *
 * @return 1-提取到数据 0-未提取到数据
 */
uint8_t BSP_ExtractEffectiveData(uint16_t *data,
                                 uint16_t *result,
                                 uint16_t data_len,
                                 uint16_t bitlen,
                                 uint16_t headtaillen)
{
    uint8_t lock = 0;
    uint16_t Ones = 0;
    uint16_t cache = 0;

    for (uint16_t i = 0; i < data_len; i++)
    {
        /* 帧头检测 */
        if ((*(data + i + 1) - *(data + i)) == 1) /* 上升沿检测 */
        {
            /* Ones自锁 */
            lock = 1;
            while (lock)
            {
                Ones++;
                i++;
                if (!*(data + i)) /* 下降沿检测 */
                    lock = 0;
            }
            /* 帧头判断 */
            if (Ones > headtaillen)
            {
                /* 1值累加器清零 */
                Ones = 0;
                for (; i < data_len;)
                {
                    /* 帧尾检测 */
                    if (*(data + 1)) /* 上升沿检测 */
                    {
                        /* Ones自锁 */
                        lock = 1;
                        while (lock)
                        {
                            Ones++;
                            i++;
                            if (!*(data + i)) /* 下降沿检测 */
                                lock = 0;
                        }
                        /* 帧尾判断 */
                        if (Ones > headtaillen + bitlen)
                        {
                            /* 数据偏移 */
                            uint16_t excursion = Ones + cache - bitlen;

                            cache += bitlen;
                            /* 数据提取 */
                            *(result + 0) = 2;
                            for (uint16_t j = 1; j < cache; j++)
                            {
                                /* 数据提取 */
                                *(result + j) = *(data + i - excursion + j);
                            }
                            *(result + cache) = 2;
                            /* Success */
                            return 1;
                        }
                        else if (Ones > headtaillen)
                        {
                            /* 数据偏移 */
                            uint16_t excursion = Ones + cache;

                            /* 数据提取 */
                            *(result + 0) = 1;
                            for (uint16_t j = 1; j < cache; j++)
                            {
                                /* 数据提取 */
                                *(result + j) = *(data + i - excursion + j);
                            }
                            *(result + cache) = 1;
                            /* Success */
                            return 1;
                        }
                        else
                        {
                            /* 数据缓存 */
                            cache += Ones;
                            /* 1累加器清零 */
                            Ones = 0;
                        }
                    }
                    else
                    {
                        /* 数据缓存 */
                        cache++;
                        i++;
                    }
                }
            }
            else
            {
                /* 1累加器清零 */
                Ones = 0;
            }
        }
    }
    /* Faile */
    return 0;
}

/**
 * @brief 帧数据提取
 *
 * @version 1.1
 * @param data 源数据
 * @param result 结果
 * @param data_len 数据长度
 * @param bitlen 位长
 * @param headtaillen 帧头/尾宽度
 * @return uint8_t 1-提取到数据 0-未提取到数据
 */
uint8_t BSP_ExtractEffectiveData_v2(uint16_t *data,
                                    uint16_t *result,
                                    uint16_t data_len,
                                    uint16_t bitlen,
                                    uint16_t headtaillen)
{
    uint16_t place = 0;
    uint16_t Ones = 0;
    uint16_t cache = 0;

    while (place < data_len)
    {
        do
        {
            Ones++;
            place++;
            /* 下降沿判断 */
        } while (__BSP_FALEDGE_DET(*(data + place), *(data + place + 1)));
        if (Ones >= headtaillen)
        {
            /* 记录回溯锚点 */
            cache = place;
            /* 1累加器清零 */
            Ones = 0;
            /* 上升沿判断 */
            if (__BSP_TOPEDGE_DET(*(data + place), (*(data + place + 1))))
            {
                do
                {
                    if (*(data + place))
                        Ones++;
                    if (Ones >= headtaillen)
                    {
                        // TODO:代码未写完 -> 1.帧尾判断 2.帧提取
                        break;
                    }
                    /* 下降沿判断 */
                } while (__BSP_FALEDGE_DET(*(data + place), *(data + place + 1)));
            }
            else
            {
                /* 向右滑动 */
                place++;
            }
        }
    }
    return 0;
}

/**
 * @brief 帧数据提取
 *
 * @version 1.2
 * @param data 源数据
 * @param result 结果
 * @param data_len 源数据长度
 * @param frame_len 帧长度
 * @return uint8_t 1-提取到数据 0-未提取到数据
 */
uint8_t BSP_ExtractEffectiveData_v3(uint16_t *data,
                                        uint16_t *result,
                                        uint16_t data_len,
                                        uint16_t frame_len)
{
    uint16_t place = 0;
    
    /* 向右滑动找到帧头 */
    do
    {
        place++;
    } while (!__BSP_FALEDGE_DET(*(data + place), *(data + place + 1)));
    /* 帧数据提取 */
    for (uint16_t i = 0; i < frame_len; i++)
    {
        result[i] = data[place + i];
        if (place + i >= data_len){
            /* Faile */
            return 0;
        }
    }
    /* success */
    return 1;
}

/**
 * @brief 数据规范化
 *
 * @param data 源数据
 * @param datalen 数据长度
 * @param bitlen 位宽
 * @param framenum 帧长度
 */
void BSP_DataStandardization(uint16_t *data, uint16_t datalen, uint16_t bitlen, uint8_t framenum)
{
    uint16_t Adder = 0;

    for (uint16_t i = 0; i < bitlen * framenum; i += bitlen)
    {
        for (uint16_t j = 1; j < bitlen; j++)
        {
            if (*(data + j + i))
                Adder++;
        }
        if (Adder >= bitlen / 2)
        {
            for (uint16_t j = 0; j < bitlen; j++)
                *(data + j + i) = 1;
        }
        else
        {
            for (uint16_t j = 0; j < bitlen; j++)
                *(data + j + i) = 0;
        }
        Adder = 0;
    }
}

/**
 * @brief 数组定幅采样
 *
 * @param data 源数据
 * @param result 结果
 * @param datalen 数据长度
 * @param bitlen 位宽
 * @param framenum 帧长度
 */
void BSP_ReadcodeFromArry(uint16_t *data, uint8_t *result, uint16_t datalen, uint16_t bitlen, uint8_t framenum)
{
    uint16_t i = 0, j = 0;

    for (i = bitlen / 2; i < datalen; i += bitlen)
    {
        if (*(data + i))
            *(result + j) = 1;
        else
            *(result + j) = 0;
        j++;
    }
}

/**
 * @brief 边沿对累积清零算法
 * @param data 源数据
 * @param result 结果
 * @param datalen 数据长度
 * @param threshold 阈值
 */
void BSP_CumMonitorEdgePairs(uint16_t *data, uint16_t *result, uint16_t datalen, uint16_t threshold)
{
    uint16_t place = 0;
    uint16_t cache = 0;
    uint16_t sum = 0;

    while (place < datalen)
    {
        /* 上升沿检测 */
        if (__BSP_TOPEDGE_DET(*(data + place), *(data + place + 1)))
        {
            /* 回溯锚点记录 */
            cache = place;
            do
            {
                place++;                                                        /* 向右滑动 */
                sum++;                                                          /* 边沿对点数累加 */
            } while (!__BSP_TOPEDGE_DET(*(data + place), *(data + place + 1))); /* 下一个上升沿截至 */
            /* 回溯尺度计算 */
            cache = place - cache;
            /* 阈值比较 */
            if (sum >= threshold)
            {
                /* 回溯清零 */
                for (uint16_t i = 0; i <= cache; i++)
                {
                    result[place - cache + i] = 0;
                }
                /* 累加器清零 */
                sum = 0;
            }
            else
            {
                /* 回溯置一 */
                for (uint16_t i = 0; i <= cache; i++)
                {
                    result[place - cache + i] = 1;
                }
                /* 累加器清零 */
                sum = 0;
            }
        }
        else
        {
            /* 数值存储 */
            result[place] = data[place];
            /* 向右滑动 */
            place++;
        }
    }
}

/**
 * @brief 和值规范算法
 * 
 * @param data 源数据
 * @param result 结果
 * @param data_len 源数据长度
 * @param bits 位数
 * @param threshold 阈值
 */
void BSP_SumValueStandard(uint16_t *data, uint16_t *result, uint16_t data_len, uint8_t bits, uint16_t threshold)
{
    /* 位宽 */
    const uint16_t bit_len = data_len / bits;
    /* 和值 */
    uint16_t sum = 0;
    /* 计数器 */
    uint16_t counter = 0;
    /* 当前位数 */
    uint16_t nowbit = 0;

    for (uint16_t i = 0; i < data_len; i++)
    {
        /* 单位宽求和 */
        if (counter++ < bit_len)
        {
            sum += data[i];
        }
        else
        {
            /* 计数器清零 */
            counter = 0;
            /* 阈值判断 */
            if (sum < threshold)
            {
                /* 回溯置 0 */
                for (uint16_t j = nowbit * bit_len; j < (nowbit + 1) * bit_len; j++)
                {
                    result[j] = 0;
                }
            }
            else
            {
                /* 回溯置 1 */
                for (uint16_t j = nowbit * bit_len; j < (nowbit + 1) * bit_len; j++)
                {
                    result[j] = 1;
                }
            }
            /* 和值清零 */
            sum = 0;
            /* 当前位数加一 */
            nowbit++;
        }
    }
}

/**
 * @brief 奇校验解码
 * @param ori_data 源数据(奇校验编码)
 * @param out_date 解码后数据
 * @param len 源数据长度
 * @return uint8_t 1-解码成功 0-解码失败
 */
uint8_t BSP_ParityCheck_OddDecode(uint8_t *ori_data, uint8_t *out_date, uint8_t len)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < len - 1; i++)
    {
        if (*(ori_data + i) == 1)
        {
            count++;
        }
    }
    return (count % 2) != *(ori_data + len - 1);
}

/**
 * @brief 偶校验解码
 * @param ori_data 源数据(奇校验编码)
 * @param out_date 解码后数据
 * @param len 源数据长度
 * @return uint8_t 1-解码成功 0-解码失败
 */
uint8_t BSP_ParityCheck_EvenDecode(uint8_t *ori_data, uint8_t *out_date, uint8_t len)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < len - 1; i++)
    {
        if (*(ori_data + i) == 1)
        {
            count++;
        }
    }
    return (count % 2) == *(ori_data + len - 1);
}

/**
 * @brief 汉明码解码（包含1位纠错功能）
 * @param received_data 接收到的编码数据（汉明码）
 * @param decoded_data 解码后的原始数据
 * @param data_len 数据长度
 * @param type 奇偶校验类型 0-偶校验 1-奇校验
 * @return true 解码成功，false 解码失败（无法纠正错误）
 */
uint8_t BSP_HammingDecode(uint8_t *received_data, uint8_t *decoded_data, const uint8_t data_len, uint8_t type)
{
    uint8_t parityLen = 0;
    while (data_len + parityLen + 1 > pow(2, parityLen))
    {
        parityLen++;
    }

    uint8_t temp_data[data_len + parityLen];
    for (uint8_t i = 0; i < data_len + parityLen; i++)
    {
        temp_data[i] = received_data[i];
    }

    uint8_t errorPos = 0;
    for (uint8_t i = 0; i < parityLen; i++)
    {
        uint8_t pos = 1 << i;
        uint8_t count = 0;

        for (uint8_t j = 0; j < data_len + parityLen; j++)
        {
            if ((((j + 1) & pos) == pos) && ((j + 1) != pos))
            {
                if (temp_data[j] == 1)
                {
                    count++;
                }
            }
        }

        if ((type && ((count + 1) % 2 != temp_data[pos - 1])) || (!type && (count % 2 != temp_data[pos - 1])))
        {
            errorPos += pos;
        }
    }

    if (errorPos > 0)
    {
        temp_data[errorPos - 1] = (temp_data[errorPos - 1] + 1) % 2;
    }
    uint8_t j = 0, y = 0;
    for (uint8_t i = 0; i < data_len + parityLen; i++)
    {
        if (i == (1 << y) - 1)
        {
            y++;
        }
        else
        {
            decoded_data[j] = temp_data[i];
            j++;
        }
    }
    return errorPos == 0;
}
