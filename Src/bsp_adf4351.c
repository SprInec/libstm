/**********************************************************
					   康威电子

功能：ADF4351正弦波点频输出，范围35M-4400M，步进0.1M，stm32f103rct6控制
			显示：12864
接口：CLK-PC11 DATA-PC10 LE-PC9 CE-PC12  按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "bsp_adf4351.h"

// #define
#define ADF4351_R0 ((uint32_t)0X2C8018)
#define ADF4351_R1 ((uint32_t)0X8029)
#define ADF4351_R2 ((uint32_t)0X10E42)
#define ADF4351_R3 ((uint32_t)0X4B3)
#define ADF4351_R4 ((uint32_t)0XEC803C)
#define ADF4351_R5 ((uint32_t)0X580005)

#define ADF4351_R1_Base ((uint32_t)0X8001)
#define ADF4351_R4_Base ((uint32_t)0X8C803C)
#define ADF4351_R4_ON ((uint32_t)0X8C803C)
#define ADF4351_R4_OFF ((uint32_t)0X8C883C)

// #define ADF4351_RF_OFF	((uint32_t)0XEC801C)

#define ADF4351_PD_ON ((uint32_t)0X10E42)
#define ADF4351_PD_OFF ((uint32_t)0X10E02)

#if 0
void delay_us(int length)
{
	while (length > 0)
		length--;
}
#endif

void WriteToADF4351(uint8_t count, uint8_t *buf)
{
	uint8_t ValueToWrite = 0;
	uint8_t i = 0;
	uint8_t j = 0;

	//	ADF_Output_GPIOInit();

	ADF4351_CE(1);
	delay_us(1);
	ADF4351_CLK(0);
	ADF4351_LE(0);
	delay_us(1);

	for (i = count; i > 0; i--)
	{
		ValueToWrite = *(buf + i - 1);
		for (j = 0; j < 8; j++)
		{
			if (0x80 == (ValueToWrite & 0x80))
			{
				ADF4351_OUTPUT_DATA(1);
			}
			else
			{
				ADF4351_OUTPUT_DATA(0);
			}
			delay_us(1);
			ADF4351_CLK(1);
			delay_us(1);
			ValueToWrite <<= 1;
			ADF4351_CLK(0);
		}
	}
	ADF4351_OUTPUT_DATA(0);
	delay_us(1);
	ADF4351_LE(1);
	delay_us(1);
	ADF4351_LE(0);
}

/*
void ReadToADF4351(uint8_t count, uint8_t *buf)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t iTemp = 0;
	uint8_t RotateData = 0;

	//ADF_Input_GPIOInit();

	ADF4351_CE(1);
	delay_us(1);
	ADF4351_CLK(0);
	ADF4351_LE(0);
	delay_us(1);

	for(i = count; i>0; i--)
	{
		for(j = 0; j<8; j++)
		{
			RotateData <<=1;
			delay_us(1);
			iTemp = ADF4351_INPUT_DATA;
			ADF4351_CLK(1);
			if(0x01 == (iTemp&0x01))
			{
				RotateData |= 1;
			}
			delay_us(1);
			ADF4351_CLK(0);
		}
		*(buf+i-1) = RotateData;
	}
	delay_us(1);
	ADF4351_LE(1);
	delay_us(1);
	ADF4351_LE(0);
}
*/

void ADF4351Init(void)
{
	uint8_t buf[4] = {0, 0, 0, 0};

	// ADF_Output_GPIOInit();

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00; // write communication register 0x00580005 to control the progress
	buf[0] = 0x05; // to write Register 5 to set digital lock detector
	WriteToADF4351(4, buf);

	buf[3] = 0x00;
	buf[2] = 0xec; //(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80; //(DB11=0)VCO powerd up;
	buf[0] = 0x3C; //(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4351(4, buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04; //(DB14-3:96H)clock divider value is 150.
	buf[0] = 0xB3;
	WriteToADF4351(4, buf);

	buf[3] = 0x00;
	buf[2] = 0x01;			//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x0E;			//(DB8=0)enable fractional-N digital lock detect;
	buf[0] = 0x42;			//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4351(4, buf); //(DB23-14:1H)R counter is 1

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x80;			//(DB14-3:6H)MOD counter is 6;
	buf[0] = 0x29;			//(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4351(4, buf); // nor the spurious optimization functions are being used
							//(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x2c;
	buf[1] = 0x80;
	buf[0] = 0x18;			//(DB14-3:0H)FRAC value is 0;
	WriteToADF4351(4, buf); //(DB30-15:140H)INT value is 320;
}
void WriteOneRegToADF4351(uint32_t Regster)
{
	uint8_t buf[4] = {0, 0, 0, 0};
	buf[3] = (uint8_t)((Regster >> 24) & (0X000000FF));
	buf[2] = (uint8_t)((Regster >> 16) & (0X000000FF));
	buf[1] = (uint8_t)((Regster >> 8) & (0X000000FF));
	buf[0] = (uint8_t)((Regster) & (0X000000FF));
	WriteToADF4351(4, buf);
}
void ADF4351_Init_some(void)
{
	WriteOneRegToADF4351(ADF4351_R2);
	WriteOneRegToADF4351(ADF4351_R3);
	WriteOneRegToADF4351(ADF4351_R5);
}

void ADF4351WriteFreq(float Fre) //	fre单位MHz -> (xx.x) M Hz
{
	uint16_t Fre_temp, N_Mul = 1, Mul_Core = 0;
	uint16_t INT_Fre, Frac_temp, Mod_temp, i;
	uint32_t W_ADF4351_R0 = 0, W_ADF4351_R1 = 0, W_ADF4351_R4 = 0;
	float multiple;

	if (Fre < 35.0)
		Fre = 35.0;
	if (Fre > 4400.0)
		Fre = 4400.0;
	Mod_temp = 1000;
	Fre = ((float)((uint32_t)(Fre * 10))) / 10;

	Fre_temp = Fre;
	for (i = 0; i < 10; i++)
	{
		if (((Fre_temp * N_Mul) >= 2199.9) && ((Fre_temp * N_Mul) <= 4400.1))
			break;
		Mul_Core++;
		N_Mul = N_Mul * 2;
	}

	multiple = (Fre * N_Mul) / FREQ_INSOURCE; // 25：鉴相频率，板载100M参考，经寄存器4分频得25M鉴相。若用户更改为80M参考输入，需将25改为20；10M参考输入，需将25改为2.5，以此类推。。。
	INT_Fre = (uint16_t)multiple;
	Frac_temp = ((uint32_t)(multiple * 1000)) % 1000;
	while (((Frac_temp % 5) == 0) && ((Mod_temp % 5) == 0))
	{
		Frac_temp = Frac_temp / 5;
		Mod_temp = Mod_temp / 5;
	}
	while (((Frac_temp % 2) == 0) && ((Mod_temp % 2) == 0))
	{
		Frac_temp = Frac_temp / 2;
		Mod_temp = Mod_temp / 2;
	}

	Mul_Core %= 7;
	W_ADF4351_R0 = (INT_Fre << 15) + (Frac_temp << 3);
	W_ADF4351_R1 = ADF4351_R1_Base + (Mod_temp << 3);
	W_ADF4351_R4 = ADF4351_R4_ON + (Mul_Core << 20);

	// WriteOneRegToADF4351(ADF4351_PD_OFF); //ADF4351_RF_OFF
	// WriteOneRegToADF4351((uint32_t)(ADF4351_R4_OFF + (6<<20)));
	WriteOneRegToADF4351(ADF4351_RF_OFF);
	WriteOneRegToADF4351(W_ADF4351_R0);
	WriteOneRegToADF4351(W_ADF4351_R1);
	WriteOneRegToADF4351(W_ADF4351_R4);
	// WriteOneRegToADF4351(ADF4351_PD_ON);
}
