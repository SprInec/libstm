/**
 * @file bsp_dht11.c
 * @author fire
 * @brief DHT11 Hardware Support
 * @version 0.1
 * @date 2023.08.21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_dht11.h"

// /**
//  * @brief 配置DHT11用到的I/O口
//  */
// void DHT11_GPIO_Config(void)
// {
// 	/*定义一个GPIO_InitTypeDef类型的结构体*/
// 	GPIO_InitTypeDef GPIO_InitStruct;

// 	/*开启DHT11相关的GPIO外设时钟*/
// 	DHT11_GPIO_CLK_ENABLE();
// 	/*选择要控制的GPIO引脚*/
// 	GPIO_InitStruct.Pin = DHT11_PIN;

// 	/*设置引脚的输出类型为推挽输出*/
// 	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

// 	/*设置引脚为上拉模式*/
// 	GPIO_InitStruct.Pull = GPIO_NOPULL;

// 	/*设置引脚速率为高速 */
// 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

// 	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
// 	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
// }

// /*
//  * 函数名：DHT11_Mode_IPU
//  * 描述  ：使DHT11-DATA引脚变为上拉输入模式
//  */
// static void DHT11_Mode_IPU(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStruct;

// 	/*选择要控制的DHT11_PORT引脚*/
// 	GPIO_InitStruct.Pin = DHT11_PIN;

// 	/*设置引脚模式为浮空输入模式*/
// 	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

// 	GPIO_InitStruct.Pull = GPIO_NOPULL;

// 	/*设置引脚速率为高速 */
// 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

// 	/*调用库函数，初始化DHT11_PORT*/
// 	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
// }

// /*
//  * 函数名：DHT11_Mode_Out_PP
//  * 描述  ：使DHT11-DATA引脚变为推挽输出模式
//  */
// static void DHT11_Mode_Out_PP(void)
// {

// 	GPIO_InitTypeDef GPIO_InitStruct;

// 	/*选择要控制的DHT11_PORT引脚*/
// 	GPIO_InitStruct.Pin = DHT11_PIN;

// 	/*设置引脚的输出类型为推挽输出*/
// 	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

// 	GPIO_InitStruct.Pull = GPIO_NOPULL;

// 	/*设置引脚速率为高速 */
// 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

// 	/*调用库函数，初始化DHT11_PORT*/
// 	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
// }

// /*
//  * 从DHT11读取一个字节，MSB先行
//  */
// static uint8_t Read_Byte(void)
// {
// 	uint8_t i, temp = 0;

// 	for (i = 0; i < 8; i++)
// 	{
// 		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/
// 		while (DHT11_DATA_IN() == Bit_RESET);

// 		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
// 		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时
// 		 */
// 		delay_ms(1); // 延时x us 这个延时需要大于数据0持续的时间即可

// 		if (DHT11_DATA_IN() == Bit_SET) /* x us后仍为高电平表示数据“1” */
// 		{
// 			/* 等待数据1的高电平结束 */
// 			while (DHT11_DATA_IN() == Bit_SET);

// 			temp |= (uint8_t)(0x01 << (7 - i)); // 把第7-i位置1，MSB先行
// 		}
// 		else // x us后为低电平表示数据“0”
// 		{
// 			temp &= (uint8_t) ~(0x01 << (7 - i)); // 把第7-i位置0，MSB先行
// 		}
// 	}
// 	return temp;
// }
// /*
//  * 一次完整的数据传输为40bit，高位先出
//  * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和
//  */
// uint8_t BSP_DHT11_ReadData(BSP_DHT11Data_TypeDef *DHT11_Data)
// {
// 	uint16_t count;
// 	/*输出模式*/
// 	DHT11_Mode_Out_PP();
// 	/*主机拉低*/
// 	DHT11_DATA_OUT(DHT11_LOW);
// 	/*延时18ms*/
// 	delay_ms(20);

// 	/*总线拉高 主机延时30us*/
// 	DHT11_DATA_OUT(DHT11_HIGH);

// 	delay_ms(1); // 延时30us

// 	/*主机设为输入 判断从机响应信号*/
// 	DHT11_Mode_IPU();

// 	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/
// 	if (DHT11_DATA_IN() == Bit_RESET)
// 	{
// 		count = 0;
// 		/*轮询直到从机发出 的80us 低电平 响应信号结束*/
// 		while (DHT11_DATA_IN() == Bit_RESET)
// 		{
// 			count++;
// 			if (count > 1000)
// 				return 0;
// 			delay_ms(1);
// 		}

// 		count = 0;
// 		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
// 		while (DHT11_DATA_IN() == Bit_SET)
// 		{
// 			count++;
// 			if (count > 1000)
// 				return 0;
// 			delay_ms(1);
// 		}
// 		/*开始接收数据*/
// 		DHT11_Data->humi_int = Read_Byte();

// 		DHT11_Data->humi_deci = Read_Byte();

// 		DHT11_Data->temp_int = Read_Byte();

// 		DHT11_Data->temp_deci = Read_Byte();

// 		DHT11_Data->check_sum = Read_Byte();

// 		/*读取结束，引脚改为输出模式*/
// 		DHT11_Mode_Out_PP();
// 		/*主机拉高*/
// 		DHT11_DATA_OUT(DHT11_HIGH);

// 		/*检查读取的数据是否正确*/
// 		if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
// 			return 1;
// 		else
// 			return 0;
// 	}
// 	else
// 	{
// 		return 0;
// 	}
// }

// /**
//  * @brief 初始化DHT11
//  */
// void BSP_DHT11_Init(void)
// {
// 	DHT11_GPIO_Config();
// }

uint16_t tem;
uint16_t him;

void DHT11_IO_IN(void) // 初始化接口定义输入和推挽输出
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = DHT11_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

void DHT11_IO_OUT(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = DHT11_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}
// 初始化DHT11的IO口 DQ 同时检测DHT11的存在
// 返回1:不存在
// 返回0:存在
uint8_t DHT11_Init(void)
{
	DHT11_Rst();
	return DHT11_Check();
}
// 复位DHT11
void DHT11_Rst(void)
{
	DHT11_IO_OUT();	   // 设置为输出
	DHT11_DQ_OUT_LOW;  // 拉低DQ
	delay_ms(20);	   // 拉低至少18ms
	DHT11_DQ_OUT_HIGH; // DQ=1
	delay_us(30);	   // 主机拉高20~40us
}

// 等待DHT11的回应
// 返回1:未检测到DHT11的存在
// 返回0:存在
uint8_t DHT11_Check(void)
{
	uint8_t retry = 0;
	DHT11_IO_IN();					   // 设置为输出
	while (DHT11_DQ_IN && retry < 100) // DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};
	if (retry >= 100)
		return 1;
	else
		retry = 0;
	while (!DHT11_DQ_IN && retry < 100) // DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	//	printf("0000!!!");
	if (retry >= 100)
		return 1;
	//	printf("11111!!!");
	return 0;
}

// 从DHT11读取一个位
// 返回值：1/0
uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0;
	while (DHT11_DQ_IN && retry < 100) // 等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry = 0;
	while (!DHT11_DQ_IN && retry < 100) // 等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40); // 等待40us
	if (DHT11_DQ_IN)
		return 1;
	else
		return 0;
}

// 从DHT11读取一个字节
// 返回值：读到的数据
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}
// 从DHT11读取一次数据
// temp:温度值(范围:0~50°)
// humi:湿度值(范围:20%~90%)
// 返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(uint16_t *temp, uint16_t *humi)
{
	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) // 读取40位数据
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			*humi = (buf[0] << 8) + buf[1];
			*temp = (buf[2] << 8) + buf[3];
		}
	}
	else
		return 1;
	return 0;
}
