/**
 * @file bsp_ina226.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2025.01.10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "bsp_ina226.h"

INA226 INA226_data;

void BSP_INA226_Init(void)
{
	BSP_I2C_Init(INA226_I2C_PORT, INA226_SDA_PIN, INA226_SCL_PIN);
	delay_ms(10);
	
	BSP_INA226_SendData(INA226_ADDR1, CFG_REG, 0x484F); // 设置转换时间204us,求平均值次数128，采样时间为204*128，设置模式为分流和总线连续模式
	BSP_INA226_SendData(INA226_ADDR1, CAL_REG, 0x0012); // 设置分流电压转电流转换参数
}

void BSP_INA226_SetRegPointer(uint8_t addr, uint8_t reg)
{
	BSP_I2C_Start();

	BSP_I2C_Send_Byte(addr);
	BSP_I2C_Wait_Ack(0);

	BSP_I2C_Send_Byte(reg);
	BSP_I2C_Wait_Ack(1);

	BSP_I2C_Stop();
}

// 发送,写入
void BSP_INA226_SendData(uint8_t addr, uint8_t reg, uint16_t data)
{
	uint8_t temp = 0;
	BSP_I2C_Start();

	BSP_I2C_Send_Byte(addr);
	BSP_I2C_Wait_Ack(2);

	BSP_I2C_Send_Byte(reg);
	BSP_I2C_Wait_Ack(3);

	temp = (uint8_t)(data >> 8);
	BSP_I2C_Send_Byte(temp);
	BSP_I2C_Wait_Ack(4);

	temp = (uint8_t)(data & 0x00FF);
	BSP_I2C_Send_Byte(temp);
	BSP_I2C_Wait_Ack(5);

	BSP_I2C_Stop();
}

// 读取
uint16_t BSP_INA226_ReadData(uint8_t addr)
{
	uint16_t temp = 0;
	BSP_I2C_Start();

	BSP_I2C_Send_Byte(addr + 1);
	BSP_I2C_Wait_Ack(6);

	temp = BSP_I2C_Read_Byte(1);
	temp <<= 8;
	temp |= BSP_I2C_Read_Byte(0);

	BSP_I2C_Stop();
	return temp;
}

/*
uint8_t BSP_INA226_AlertAddr()
{
	uint8_t temp;
	BSP_I2C_Start();

	BSP_I2C_Send_Byte(INA226_GETALADDR);
	BSP_I2C_Wait_Ack();

	temp = BSP_I2C_Read_Byte(1);

	BSP_I2C_Stop();
	return temp;
}
*/

// 1mA/bit
uint16_t BSP_INA226_GetShunt_Current(uint8_t addr)
{
	uint16_t temp = 0;
	BSP_INA226_SetRegPointer(addr, CUR_REG);
	temp = BSP_INA226_ReadData(addr);
	if (temp & 0x8000)
		temp = ~(temp - 1);
	return temp;
}

// 获取 id
uint16_t BSP_INA226_Get_ID(uint8_t addr)
{
	uint32_t temp = 0;
	BSP_INA226_SetRegPointer(addr, INA226_GET_ADDR);
	temp = BSP_INA226_ReadData(addr);
	return (uint16_t)temp;
}

// 获取校准值
uint16_t BSP_INA226_GET_CAL_REG(uint8_t addr)
{
	uint32_t temp = 0;
	BSP_INA226_SetRegPointer(addr, CAL_REG);
	temp = BSP_INA226_ReadData(addr);
	return (uint16_t)temp;
}

// 1.25mV/bit
uint16_t BSP_INA226_GetVoltage(uint8_t addr)
{
	uint32_t temp = 0;
	BSP_INA226_SetRegPointer(addr, BV_REG);
	temp = BSP_INA226_ReadData(addr);
	return (uint16_t)temp;
}

// 2.5uV/bit,感觉这个值是测不准的，所以下面改成2.2了
uint16_t BSP_INA226_GetShuntVoltage(uint8_t addr)
{
	int16_t temp = 0;
	BSP_INA226_SetRegPointer(addr, SV_REG);
	temp = BSP_INA226_ReadData(addr);
	if (temp & 0x8000)
		temp = ~(temp - 1);
	return (uint16_t)temp;
}

uint16_t BSP_INA226_Get_Power(uint8_t addr)
{
	int16_t temp = 0;
	BSP_INA226_SetRegPointer(addr, PWR_REG);
	temp = BSP_INA226_ReadData(addr);
	return (uint16_t)temp;
}

void BSP_GetVoltage(float *Voltage) // mV
{
	Voltage[0] = BSP_INA226_GetVoltage(INA226_ADDR1) * 1.25f;
}

void BSP_Get_Shunt_voltage(float *Voltage) // uV
{
	Voltage[0] = (BSP_INA226_GetShuntVoltage(INA226_ADDR1) * 2.2); // 这里原来乘的系数是2.5
}

void BSP_Get_Shunt_Current(float *Current) // mA
{
	Current[0] = (BSP_INA226_GetShunt_Current(INA226_ADDR1) * 2.5f);
}

void BSP_GetPower() // W
{
	BSP_GetVoltage(&INA226_data.voltageVal);		   // mV
	BSP_Get_Shunt_voltage(&INA226_data.Shunt_voltage); // uV
	BSP_Get_Shunt_Current(&INA226_data.Shunt_Current); // mA
	INA226_data.powerVal = INA226_data.voltageVal * 0.001f * INA226_data.Shunt_Current * 0.001f;
}

/*
void BSP_Get_Power(float *Current) // W
{
	Current[0] = (BSP_INA226_Get_Power(INA226_ADDR1)*50);
}
*/
