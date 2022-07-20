/*
 ******************************************************************************
 * @file		:bsp_coding.c
 * @brief		:The board support package for digital coding.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27
 ******************************************************************************
 */
#include "bsp_coding.h"

/* 十进制转二进制 */
void bsp_DecToBin(int num, uint16_t code[], unsigned char len)
{
    for(unsigned char i = 0; i < len; i++)
        code[i] = 0;
    unsigned char j = len-1;
    while(num > 0)
    {
        code[j--] = num % 2;
        num /= 2;
    }
}

/* 浮点数转二进制 */
/* 整数小数各两位 */
void bsp_FloatToBin(float num, uint16_t code[])
{
  uint16_t Hn, Ln;
	unsigned char j = 15;
	
	Hn = (uint16_t)num;
    Ln = (((uint16_t)(num*100))%100);
	
	for(unsigned char i = 0; i < 16; i++)
        code[i] = 0; 
  
    while(Ln > 0 || j >= 8)
    {
        code[j--] = Ln % 2;
        Ln /= 2;
    }
	while(Hn > 0 || j < 8)
	{
		code[j--] = Hn % 2;
		Hn /= 2;
	}
}

#ifdef __RZ
/* 归零编码 RZ */
_Bool RZ(uint16_t str[], uint16_t code[], char len)
{
    char p = 2;
    uint16_t temp;
    for(int i = 0; i < len*2+2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;

    for(char i = 0; i < len; i++)
    {
        if(str[i] == 0){
            code[p++] = code[p-2];
            code[p++] = code[p-2];
        }
        else if(str[i] == 1){
            code[p++] = !code[p-2];
            temp = code[p-1];
            code[p++] = temp;
        }
        else{
            //printf("\ndata error!");
            return 1;
        }
    }
//    code[p] = 0;
    return 0;
}
#endif


#ifdef __NRZ
/* 不归零编码 NRZ */
_Bool NRZ(uint16_t str[], uint16_t code[], char len)
{
    for(int i = 0; i < len*2+2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;

    for(char i = 0; i < len; i++)
    {
        if(str[i] == 0){
            code[p++] = 0;
            code[p++] = 0;
        }
        else if(str[i] == 1){
            code[p++] = 1;
            code[p++] = 1;
        }
        else{
            //printf("\ndata error!");
            return 1;
        }
    }
//    code[p] = 0;
    return 0;
}
#endif

#ifdef __MCST
/* 曼彻斯特编码 MCST */
_Bool MCST(uint16_t str[], uint16_t code[], char len)
{
    for(int i = 0; i < len*2+2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;

    for(char i = 0; i < len; i++)
    {
        if(str[i] == 0){
            code[p++] = 0;
            code[p++] = 1;
        }
        else if(str[i] == 1){
            code[p++] = 1;
            code[p++] = 0;
        }
        else{
            //printf("\ndata error!");
            return 1;
        }
    }
//    code[p] = 0;
    return 1;
}

/* 曼彻斯特解码 */
_Bool MCST_Decode(uint16_t code[], uint16_t decode[])
{
	if(code[0] == 0 && code[1] == 1)
	{
		for(char i = 0, j = 0; i < 16; i+=2)
		{	
			if(code[i+2] == 0 && code[i+3] == 1)
			{
				decode[j++] = 0;
			}
			else if(code[i+2] == 1 && code[i+3] == 0)
			{
				decode[j++] = 1;
			}
		}
		return 1;
	}
	else{
		return 0;
	}
}
#endif

#ifdef __DFMCST
/* 差分曼彻斯特编码 DFMCST */
_Bool DFMCST(uint16_t str[], uint16_t code[], char len)
{
    for(int i = 0; i < len*2+2; i++)
        code[i] = 0;
    code[0] = 0;
    code[1] = 1;
    char p = 2;
    uint16_t temp;

    for(char i = 0; i < len; i++)
    {
        if(str[i] == 0){
            temp = code[p-2];
            code[p++] = temp;
            temp = code[p-2];
            code[p++] = temp;
        }
        else if(str[i] == 1){
            temp = code[p-1];
            code[p++] = temp;
            temp = code[p-3];
            code[p++] = temp;
        }
        else{
            //printf("\ndata error!");
            return 1;
        }
    }
//    code[p] = 0;
    return 0;
}
#endif

