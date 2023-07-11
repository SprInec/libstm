/*
 ******************************************************************************
 * @file		:bsp_adf4002.c
 * @brief		:ADF4002 Driver.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.07.27
 ******************************************************************************
 * @pinset
 *				PC6		--> SCK
 *				PC7		--> SDA/SDI
 *				PC8		--> LE/SEN
 *
 ******************************************************************************
 */
#include "bsp_adf4002.h"

#ifdef USE_ADF4002_VER_1

long int ReadData;

#if 0
long int Reg0x00 = 0x001F40;	//r
long int Reg0x01 = 0x0DAC01;	//n
#endif

long int Reg0x02 = 0x0D80C2; // function
long int Reg0x03 = 0x0D80C3; // init
#if 0
long int Reg0x02 = 0x0D80B2; 	//function DVDD
long int Reg0x03 = 0x0D80B3;	//init DVDD
#endif

long int Reg0x02_LEDON = 0x0D80B2;	// function
long int Reg0x02_LEDOFF = 0x0D80F2; // function

void ADF4002_InitPll(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	PLL_SCK_0;
	PLL_SDI_0;
	PLL_SEN_0;
	SendDataPll(Reg0x03); // init
	SendDataPll(Reg0x02); // funtion
}
void testADF4002(void)
{
	ADF4002_InitPll();
	SendDataPll(Reg0x02_LEDOFF);
	delay_us(15);
	SendDataPll(Reg0x02_LEDON);
	delay_us(15);
	SendDataPll(Reg0x02_LEDOFF);
	delay_us(15);
	SendDataPll(Reg0x02_LEDON);
	delay_us(15);
}

/* 传送的数据 */
void SendDataPll(unsigned long int Data)
{
	unsigned char i;

	PLL_SEN_0;
	PLL_SCK_0;

	for (i = 0; i < 24; i++)
	{
		if (Data & 0x800000)
		{
			PLL_SDI_1;
		}
		else
		{
			PLL_SDI_0;
		}
		Data <<= 1;
		PLL_SCK_1;

		delay_us(100);

		PLL_SCK_0;
		delay_us(100);
	}
	PLL_SDI_0;

	PLL_SEN_0;
	delay_us(100);
	PLL_SEN_1;
#if 0
  delay_us(100);
  PLL_SEN_0;
#endif
}

/* 设置频率 */
void SetFrequency(uint32_t fvco)
{
	uint16_t NN;
	double bl;
	uint16_t R = 1; // R分频
	uint32_t S_R = 0, S_N = 0;
	bl = ((double)fvco) / 100000000;
	NN = (uint16_t)(bl * R);
	S_R = Pre_R + (R << 2) + R_Address;
	S_N = Pre_N + (NN << 8) + N_Address;
	SendDataPll(S_R);
	SendDataPll(S_N);
	SendDataPll(Reg0x02); // funtion
	SendDataPll(Reg0x03); // init
}

void RDivideTest(uint16_t RData)
{
	uint16_t NN;
	uint32_t S_R = 0, S_N = 0;
	double bl = 1;

	NN = (uint16_t)(bl * RData);
	S_R = Pre_R + (RData << 2) + R_Address;
	S_N = Pre_N + (NN << 8) + N_Address;
	SendDataPll(S_R);
	SendDataPll(S_N);
	SendDataPll(Reg0x02); // funtion
	SendDataPll(Reg0x03); // init
	SendDataPll(S_N);
	SendDataPll(S_R);
}
#endif

#ifdef USE_ADF4002_VER_2

uint32_t register_r = 0;
uint32_t register_n = 0;

uint64_t R_register_slop = 100000;

void ADF4002_PinInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ADF4002_Init()
{
	ADF4002_PinInit();

	register_r = CTRL_R_COUNTER | RCOUNTER_ABP_2_9_ns;
	register_n = CTRL_N_COUNTER | NCOUNTER_G1;

#if 0
	ADF4002_WriteLatch(CTRL_INIT_LATCH|INIT_PD_DISABLE|INIT_CPI2(0)|INIT_CPI1(0)|INIT_TC(0)|INIT_FASTLOCK_DISABLE|INIT_MUX_N_DIVIDER);
	ADF4002_WriteLatch(CTRL_FUNC_LATCH|INIT_PD_DISABLE|INIT_CPI2(0)|INIT_CPI1(0)|INIT_TC(0)|INIT_FASTLOCK_DISABLE|INIT_MUX_N_DIVIDER);
	ADF4002_WriteLatch(CTRL_INIT_LATCH|0x1C2680);
	ADF4002_WriteLatch(CTRL_FUNC_LATCH|0x1C2680);
#endif

	ADF4002_WriteLatch(0x1C26A3);
	ADF4002_WriteLatch(0x1C26A2);

	ADF4002_SetRCounter(100);
	ADF4002_SetNCounter(2010); // 2010 TO 2410
}
void ADF4002_SetRCounter(uint16_t _num)
{
	ADF4002_WriteLatch(register_r | ((0x3FFF & _num) << 2));
	switch (_num)
	{
	case 100:
		R_register_slop = 100000;
		break;
	case 200:
		R_register_slop = 50000;
		break;
	case 400:
		R_register_slop = 25000;
		break;
	}
}
void ADF4002_SetNCounter(uint16_t _num)
{
	ADF4002_WriteLatch(register_n | ((0x1FFF & _num) << 8));
}
void ADF4002_WriteLatch(uint32_t _register)
{
	uint8_t i;
	ADF4002_SCK_0;
	delay_us(0x0F);
	ADF4002_LE_0;
	delay_us(0x0F);
	for (i = 0; i < 24; i++)
	{

		if (_register & 0x800000)
		{
			ADF4002_SDA_1;
		}
		else
		{
			ADF4002_SDA_0;
		}
		delay_us(0x0F);
		ADF4002_SCK_1;
		delay_us(0x0F);
		ADF4002_SCK_0;
		_register <<= 1;
		delay_us(0x0F);
	}
	ADF4002_LE_1;
	delay_us(0x0F);
	ADF4002_SCK_1;
	delay_us(0x0F);
}

void ADF4002_SetFrequency(uint64_t _freq)
{
	uint16_t tt;
	tt = _freq / 100000;
	ADF4002_SetNCounter(tt);
}
#endif
