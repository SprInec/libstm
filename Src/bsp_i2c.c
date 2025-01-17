/**
 * @file bsp_i2c.c
 * @author SprInec
 * @brief 
 * @version 0.1
 * @date 2025.01.10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "bsp_i2c.h"

/**
 * @brief 初始化 I2C
 * 
 * @param I2C_Port I2C 端口
 * @param I2C_SCL_Pin I2C SCL 引脚
 * @param I2C_SDA_Pin I2C SDA 引脚
 */
void BSP_I2C_Init(GPIO_TypeDef *I2C_Port, uint16_t I2C_SCL_Pin, uint16_t I2C_SDA_Pin)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (I2C_Port == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (I2C_Port == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (I2C_Port == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if (I2C_Port == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (I2C_Port == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	   
	GPIO_InitStructure.Pin = I2C_SCL_Pin | I2C_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
	HAL_GPIO_Init(I2C_Port, &GPIO_InitStructure);
 
	I2C_SCL = 1;
	I2C_SDA = 1;
}

/**
 * @brief 发送 I2C 开始信号
 * 
 */
void BSP_I2C_Start(void)
{
	SDA_OUT();     //sda线输出
	I2C_SDA = 1;	  	  
	I2C_SCL = 1;
	delay_us(4);
 	I2C_SDA = 0; // START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL = 0; // 钳住I2C总线，准备发送或接收数据 
}	  

/**
 * @brief 发送 I2C 停止信号
 * 
 */
void BSP_I2C_Stop(void)
{
	SDA_OUT();//sda线输出
	I2C_SCL = 0;
	I2C_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2C_SCL = 1; 
	I2C_SDA = 1;//发送I2C总线结束信号
	delay_us(4);							   	
}

/**
 * @brief 等待应答信号到来
 * 
 * @return 1，接收应答失败
 *         0，接收应答成功
 */
u8 BSP_I2C_Wait_Ack(uint8_t id)
{
    uint32_t timeout = 0;
    const uint32_t TIMEOUT_MAX = 500;  // 根据实际情况调整超时值

	SDA_IN(); // SDA设置为输入

	I2C_SDA = 1;
    delay_us(1);

	I2C_SCL = 1;
    delay_us(1);	 

    while(READ_SDA)
    {
        timeout++;
        delay_us(1);  // 添加小延时避免死循环过快
        if(timeout >= TIMEOUT_MAX)
        {
            BSP_I2C_Stop();
            printf("ID: %d - I2C ACK Timeout!\r\n", id);  // 可选的调试输出
            return 1;
        }
    }
    
    I2C_SCL = 0;  // 时钟输出0 	   
    return 0;  
} 

/**
 * @brief 发送 I2C 应答信号
 * 
 */
void BSP_I2C_Ack(void)
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 0;
	delay_us(2);
	I2C_SCL = 1;
	delay_us(2);
	I2C_SCL = 0;
}

/**
 * @brief 不发送 I2C 应答信号
 * 
 */
void BSP_I2C_NAck(void)
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 1;
	delay_us(2);
	I2C_SCL = 1;
	delay_us(2);
	I2C_SCL = 0;
}					 				     

/**
 * @brief 发送 I2C 字节
 * 
 * @param txd 要发送的字节
 */
void BSP_I2C_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    I2C_SCL = 0; // 拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        I2C_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2C_SCL = 1;
		delay_us(2); 
		I2C_SCL = 0;	
		delay_us(2);
    }	 
} 

/**
 * @brief 读 1 个字节
 * @param ack 是否发送 ACK
 *  - ack = 1 发送 ACK
 *  - ack = 0 发送 NACK
 * @return 读取到的字节
 */
u8 BSP_I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(); // SDA设置为输入
    for(i = 0; i < 8; i++ )
	{
        I2C_SCL = 0; 
        delay_us(2);
		I2C_SCL = 1;
        receive <<= 1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        BSP_I2C_NAck();
    else
		BSP_I2C_Ack();
	return receive;
}



























