/*
 ******************************************************************************
 * @file		:bsp_coding.c
 * @brief		:The board support package for digital coding.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27	-v0
 * @updated     :2023.02.27 -v1
 * @updated     :2023.02.27 -v2
 ******************************************************************************
 */
#include "bsp_coding.h"
#ifdef __BSP_CODING_ENABLED
#include "string.h"
#include "math.h"

/* 巴克码序列 */
int bsp_baker7bit[] = {1, 1, 1, -1, -1, 1, -1};
int bsp_baker11bit[] = {1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1};
int bsp_baker13bit[] = {1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1};


/**
 * @brief arr_char转二进制
 *
 * @param num 字符型数组
 * @param code 转换后存储二进制码的数组
 * @param len 数组长度
 */
void BSP_charToBin(char *num, uint16_t *code, uint8_t len)
{
    uint8_t i = 0;
    uint8_t j = 0;

    for (; i < len; i++)
    {
        for (j = i * 4; j < (i + 1) * 4; j++)
        {
            if (*(num + i) & 0x01){
                *(code + j) = 1;
            }
            else{
                *(code + j) = 0;
            }
            *(num + i) = *(num + i) >> 1;
        }
    }
}

/**
 * @brief 十进制转二进制(BCD)
 * 
 * @param num 十进制数
 * @param code 转换后存储二进制码的数组
 * @param len 十进制数长度
 */
void BSP_DecToBCD(int num, uint16_t code[], uint8_t len)
{
    len *= 4;
    for (uint8_t i = 0; i < len; i++)
        code[i] = 0;
    uint8_t j = len - 1;
    while (num > 0)
    {
        code[j--] = num % 2;
        num /= 2;
    }
}

/**
 * @brief 浮点数转二进制(二进制限长16位)
 * @param num 待转换的浮点数，限制为两位整数+两位小数
 * @param code 转换后存储二进制码的数组，限制长度为16位
 */
void BSP_FloatTo16BCD(float num, uint16_t code[])
{
    uint16_t Hn, Ln; // Hn高位， Ln低位
    unsigned char j = 15;

    Hn = (uint16_t)num;
    Ln = (((uint16_t)(num * 100)) % 100);

    for (unsigned char i = 0; i < 16; i++)
        code[i] = 0;

    while (Ln > 0 || j >= 8)
    {
        code[j--] = Ln % 2;
        Ln /= 2;
    }
    while (Hn > 0 || j < 8)
    {
        code[j--] = Hn % 2;
        Hn /= 2;
    }
}

/**
 * @brief 浮点数转BCD
 * @param num 待转换的浮点数，限制为两位整数+四位小数
 * @param code 转换后存储二进制码的数组，限制长度为24位
 * @param 
 */
void BSP_FloatTo24BCD(float num, uint16_t code[])
{
    uint16_t Hn; // Hn高位
    uint32_t Ln1; // Ln低位
    uint32_t Ln2; // Ln低位
    unsigned char j = 23;

    Hn = (uint16_t)num;
    Ln1 = (((uint32_t)(num * 100)) % 100);
    Ln2 = ((uint32_t)(num * 10000) / 100 % 100);

    /* 数组清零 */
    for (unsigned char i = 0; i < 24; i++)
        code[i] = 0;

    while (Ln1 >= 0 && j > 15)
    {
        code[j--] = Ln1 % 2;
        Ln1 /= 2;
    }
    while(Ln2 >= 0 && j >=8)
    {
        code[j--] = Ln2 % 2;
        Ln2 /= 2;
    }
    while (Hn > 0 && j < 8)
    {
        code[j--] = Hn % 2;
        Hn /= 2;
    }
}

/**
 * @brief 浮点数转BCD(不限长)
 *
 * @param num 带转换的浮点数
 * @param codeH 存储整数位BCD码的数组
 * @param codeL 存储小数位BCD码的数组
 * @param arrlenH 整数变长数组长度变量
 * @param arrlenL 小数边长数组长度变量
 */
void BSP_FloatToBCDv2(double num,
                    uint16_t codeH[],
                    uint16_t codeL[],
                    uint16_t *arrlenH,
                    uint16_t *arrlenL)
{
    double Hn, Ln;
    uint16_t Hnn, Lnn;
    uint16_t temp, place = 0;
    uint16_t counter = 0;

    Ln = modf(num, &Hn);

    /* 整数位长度计算 */
    Hnn = (uint16_t)Hn;
    while (Hnn / pow(10, counter) > 0)
        counter++;
    *arrlenH = 4 * counter;

    /* 整数位转BCD码 */
    while (counter--)
    {
        temp = Hnn % 10;
        Hnn /= 10;
        switch (temp)
        {
        case 0:
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 0;
            break;
        case 1:
            codeH[place++] = 1;
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 0;
            break;
        case 2:
            codeH[place++] = 0;
            codeH[place++] = 1;
            codeH[place++] = 0;
            codeH[place++] = 0;
            break;
        case 3:
            codeH[place++] = 1;
            codeH[place++] = 1;
            codeH[place++] = 0;
            codeH[place++] = 0;
            break;
        case 4:
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 1;
            codeH[place++] = 0;
            break;
        case 5:
            codeH[place++] = 1;
            codeH[place++] = 0;
            codeH[place++] = 1;
            codeH[place++] = 0;
            break;
        case 6:
            codeH[place++] = 0;
            codeH[place++] = 1;
            codeH[place++] = 1;
            codeH[place++] = 0;
            break;
        case 7:
            codeH[place++] = 1;
            codeH[place++] = 1;
            codeH[place++] = 1;
            codeH[place++] = 0;
            break;
        case 8:
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 1;
            break;
        case 9:
            codeH[place++] = 1;
            codeH[place++] = 0;
            codeH[place++] = 0;
            codeH[place++] = 1;
            break;
        }
    }
    place = 0;

    /* 小数位长度计算 */
    while (Ln * pow(10, counter) - (uint16_t)(Ln * pow(10, counter)) > 0)
        counter++;
    *arrlenL = 4 * counter;
    Lnn = (uint16_t)(Ln * pow(10, counter));

    /* 小数位转BCD码 */
    while (counter--)
    {
        temp = Lnn % 10;
        Lnn /= 10;
        switch (temp)
        {
        case 0:
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 0;
            break;
        case 1:
            codeL[place++] = 1;
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 0;
            break;
        case 2:
            codeL[place++] = 0;
            codeL[place++] = 1;
            codeL[place++] = 0;
            codeL[place++] = 0;
            break;
        case 3:
            codeL[place++] = 1;
            codeL[place++] = 1;
            codeL[place++] = 0;
            codeL[place++] = 0;
            break;
        case 4:
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 1;
            codeL[place++] = 0;
            break;
        case 5:
            codeL[place++] = 1;
            codeL[place++] = 0;
            codeL[place++] = 1;
            codeL[place++] = 0;
            break;
        case 6:
            codeL[place++] = 0;
            codeL[place++] = 1;
            codeL[place++] = 1;
            codeL[place++] = 0;
            break;
        case 7:
            codeL[place++] = 1;
            codeL[place++] = 1;
            codeL[place++] = 1;
            codeL[place++] = 0;
            break;
        case 8:
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 1;
            break;
        case 9:
            codeL[place++] = 1;
            codeL[place++] = 0;
            codeL[place++] = 0;
            codeL[place++] = 1;
            break;
        }
    }
}

#if ENCODING_MODE_SELECT
/**
 * @brief 归零编码 RZ
 * @param str 源数据
 * @param code 编码结果
 * @param len 源数据长度
 * @return 0-成功 1-失败
 */
_Bool RZ(uint16_t str[], uint16_t code[], uint16_t len)
{
    char p = 2;
    uint16_t temp;

    for (int i = 0; i < len * 2 + 2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;

    for (char i = 0; i < len; i++)
    {
        if (str[i] == 0)
        {
            code[p] = code[p - 2];
            p++;
            code[p] = code[p - 2];
            p++;
        }
        else if (str[i] == 1)
        {
            code[p] = !code[p - 2];
            p++;
            temp = code[p - 1];
            code[p++] = temp;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief 不归零编码 NRZ
 * @param str 源数据
 * @param code 编码结果
 * @param len 源数据长度
 * @return 0-成功 1-失败
 */
_Bool NRZ(uint16_t str[], uint16_t code[], uint16_t len)
{
    for (int i = 0; i < len * 2 + 2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;

    for (char i = 0; i < len; i++)
    {
        if (str[i] == 0)
        {
            code[p++] = 0;
            code[p++] = 0;
        }
        else if (str[i] == 1)
        {
            code[p++] = 1;
            code[p++] = 1;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief 曼彻斯特编码 MCST
 * @param str 源数据
 * @param code 编码结果
 * @param len 源数据长度
 * @return 0-成功 1-失败
 */
_Bool MCST(uint16_t str[], uint16_t code[], uint16_t len)
{
    for (int i = 0; i < len * 2 + 2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;

    for (char i = 0; i < len; i++)
    {
        if (str[i] == 0)
        {
            code[p++] = 0;
            code[p++] = 1;
        }
        else if (str[i] == 1)
        {
            code[p++] = 1;
            code[p++] = 0;
        }
        else
        {
            return 1;
        }
    }
    return 1;
}

/**
 * @brief 曼彻斯特解码 DCMCST
 * @param str 曼彻斯特源码
 * @param code 解码结果
 * @param len 源数据长度
 * @return 0-成功 1-失败
 */
_Bool DCMCST(uint16_t code[], uint16_t decode[], uint16_t len)
{
    if (code[0] == 0 && code[1] == 1)
    {
        for (char i = 0, j = 0; i < 16; i += 2)
        {
            if (code[i + 2] == 0 && code[i + 3] == 1)
            {
                decode[j++] = 0;
            }
            else if (code[i + 2] == 1 && code[i + 3] == 0)
            {
                decode[j++] = 1;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 差分曼彻斯特编码 DFMCST
 * @param str 源数据
 * @param code 编码结果
 * @param len 源数据长度
 * @return 0-成功 1-失败
 */
_Bool DFMCST(uint16_t str[], uint16_t code[], uint16_t len)
{
    for (int i = 0; i < len * 2 + 2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;
    uint16_t temp;

    for (char i = 0; i < len; i++)
    {
        if (str[i] == 0)
        {
            temp = code[p - 2];
            code[p++] = temp;
            temp = code[p - 2];
            code[p++] = temp;
        }
        else if (str[i] == 1)
        {
            temp = code[p - 1];
            code[p++] = temp;
            temp = code[p - 3];
            code[p++] = temp;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
#endif

#endif