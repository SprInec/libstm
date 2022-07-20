/**
 ******************************************************************************
 * @file		:bsp_decode.c
 * @brief		:The board support package for decode.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.28
 ******************************************************************************
 */

#include "stdio.h"
#include "bsp_decode.h"


 /* 二进制输出 */
void print_binary(unsigned int number) {
    if (number >> 1) {
        print_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

/* 同步帧检测 */
/* 1111 1111 */
_Bool bsp_SyncDetection(unsigned int data[])
{
    unsigned char temp;

    for(char i = 7, j = 0; j < 8; i--, j++)
        temp |= data[i] << j;
    if(temp == 0xFF)
        return 1;
    else
        return 0;
}

/* 结束帧检测 */
/* 0000 0000 */
_Bool bsp_SyncEnd(unsigned int data[])
{
    unsigned int temp;

    for(char i = 7, j = 0; j < 8; i--, j++)
        temp |= data[i] << j;
    if(temp == 0x00)
        return 1;
    else
        return 0;
}

/* 接收一字节数据 */
unsigned char bsp_SyncRx1Byte(unsigned int data[])
{
    unsigned int temp;
    for(char i = 7, j = 0; j < 8; i--, j++)
        temp |= data[i] << j;
    return temp;
}

/* 获取定长数组 */
void bsp_Get1ByetfromArray(unsigned int data[], unsigned int result[], char startlen, int len)
{
    for(char i = 0; i < len; i++){
        result[i] = data[startlen + i];
    }
}

/* 接收不定长数据 */
_Bool bsp_SyncRxData(unsigned int data[], unsigned char result[])
{
    unsigned int temp[8];
    result[0] = 0;
    result[1] = 0;

    bsp_Get1ByetfromArray(data, temp, 0, 8);
    if(bsp_SyncDetection(temp))
    {
        bsp_Get1ByetfromArray(data, temp, 8, 8);
        result[0] = bsp_SyncRx1Byte(temp);
        bsp_Get1ByetfromArray(data, temp, 16, 8);
        result[1] = bsp_SyncRx1Byte(temp);
        return 1;
    }
    else
        return 0;
}


/* 16位数组转正浮点数 */
/* 高8位整数 低8位小数 */
float bsp_Arry16ToUFloat(uint16_t arry[])
{
	uint8_t temp1 = 0x00u, temp2 = 0x00u;
	float temp3, result;
	
	for(uint8_t i = 0; i < 16; i++)
	{
		if(i < 8){
			temp1 += arry[i];
			temp1 <<= 1;
		}
		else{
			temp2 += arry[i];
			temp2 <<= 1;
		}
	}
	temp3 = (float)(temp2/100.0);
	result = temp1/2 + temp3/2;
	
	return result;
}

/* 8位数组转正整数 */
uint16_t	bsp_Arry8ToUInt(uint16_t arry[])
{
	uint16_t temp = 0x0000u;
	
	for(uint8_t i = 0; i < 8; i++)
	{
		temp += arry[i];
		temp <<= 1;
	}
	
	return temp;
}


