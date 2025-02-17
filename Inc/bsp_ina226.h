/**
 * @file bsp_ina226.h
 * @author SprInec 
 * @brief 
 * @version 0.1
 * @date 2025.01.10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __BSP_INA226_H
#define __BSP_INA226_H
#include "bsp_config.h"

#define INA226_I2C_PORT GPIOB
#define INA226_SDA_PIN GPIO_PIN_7
#define INA226_SCL_PIN GPIO_PIN_6

#define BATTERY_VOLTAGE_MAX 4300
#define BATTERY_VOLTAGE_MIN 3100

#define 	CFG_REG	 		0x00		
#define 	SV_REG 			0x01		//分流电压， 此处分流电阻为 0.1欧
#define 	BV_REG 			0x02		//总线电压
#define 	PWR_REG 		0x03		//电源功率
#define 	CUR_REG 		0x04		//电流
#define 	CAL_REG 		0x05		//校准，设定满量程范围以及电流和功率测数的 
#define 	ONFF_REG 		0x06		//屏蔽 使能 警报配置和转换准备就绪
#define 	AL_REG 			0x07		//包含与所选警报功能相比较的限定值
#define 	INA226_GET_ADDR 0XFF		//包含唯一的芯片标识号
#define   	INA226_ADDR1	0x80 
//#define   	INA226_GETALADDR	0x14 

typedef struct
{
	float voltageVal;			//mV
	float Shunt_voltage;		//uV
	float Shunt_Current;		//mA
	float powerVal;

}INA226;

void BSP_INA226_Init(void);
void BSP_INA226_SetRegPointer(uint8_t addr, uint8_t reg);
void BSP_INA226_SendData(uint8_t addr, uint8_t reg, uint16_t data);

uint16_t BSP_INA226_ReadData(uint8_t addr);
//uint8_t INA226_AlertAddr(void);
uint16_t BSP_INA226_Get_ID(uint8_t addr);				//获取 id
uint16_t BSP_INA226_GetVoltage(uint8_t addr);		//获取总线电压
uint16_t BSP_INA226_GetShunt_Current(uint8_t addr);	//获取分流电流
uint16_t BSP_INA226_GetShuntVoltage(uint8_t addr);	//分流电压
uint16_t BSP_INA226_Get_Power(uint8_t addr);			//获取功率

uint16_t BSP_INA226_GET_CAL_REG(uint8_t addr);
void BSP_GetVoltage(float *Voltage);		
void BSP_Get_Shunt_voltage(float *Current);
void BSP_Get_Shunt_Current(float *Current);
//void BSP_Get_Power(float *Current);
//void BSP_GetPower();//W

extern INA226 INA226_data;
#endif /* __BSP_INA226_H */
