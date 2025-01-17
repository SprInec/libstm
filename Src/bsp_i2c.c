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
 * @brief ��ʼ�� I2C
 * 
 * @param I2C_Port I2C �˿�
 * @param I2C_SCL_Pin I2C SCL ����
 * @param I2C_SDA_Pin I2C SDA ����
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
	GPIO_InitStructure.Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
	HAL_GPIO_Init(I2C_Port, &GPIO_InitStructure);
 
	I2C_SCL = 1;
	I2C_SDA = 1;
}

/**
 * @brief ���� I2C ��ʼ�ź�
 * 
 */
void BSP_I2C_Start(void)
{
	SDA_OUT();     //sda�����
	I2C_SDA = 1;	  	  
	I2C_SCL = 1;
	delay_us(4);
 	I2C_SDA = 0; // START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL = 0; // ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

/**
 * @brief ���� I2C ֹͣ�ź�
 * 
 */
void BSP_I2C_Stop(void)
{
	SDA_OUT();//sda�����
	I2C_SCL = 0;
	I2C_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2C_SCL = 1; 
	I2C_SDA = 1;//����I2C���߽����ź�
	delay_us(4);							   	
}

/**
 * @brief �ȴ�Ӧ���źŵ���
 * 
 * @return 1������Ӧ��ʧ��
 *         0������Ӧ��ɹ�
 */
u8 BSP_I2C_Wait_Ack(uint8_t id)
{
    uint32_t timeout = 0;
    const uint32_t TIMEOUT_MAX = 500;  // ����ʵ�����������ʱֵ

	SDA_IN(); // SDA����Ϊ����

	I2C_SDA = 1;
    delay_us(1);

	I2C_SCL = 1;
    delay_us(1);	 

    while(READ_SDA)
    {
        timeout++;
        delay_us(1);  // ���С��ʱ������ѭ������
        if(timeout >= TIMEOUT_MAX)
        {
            BSP_I2C_Stop();
            printf("ID: %d - I2C ACK Timeout!\r\n", id);  // ��ѡ�ĵ������
            return 1;
        }
    }
    
    I2C_SCL = 0;  // ʱ�����0 	   
    return 0;  
} 

/**
 * @brief ���� I2C Ӧ���ź�
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
 * @brief ������ I2C Ӧ���ź�
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
 * @brief ���� I2C �ֽ�
 * 
 * @param txd Ҫ���͵��ֽ�
 */
void BSP_I2C_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    I2C_SCL = 0; // ����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        I2C_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCL = 1;
		delay_us(2); 
		I2C_SCL = 0;	
		delay_us(2);
    }	 
} 

/**
 * @brief �� 1 ���ֽ�
 * @param ack �Ƿ��� ACK
 *  - ack = 1 ���� ACK
 *  - ack = 0 ���� NACK
 * @return ��ȡ�����ֽ�
 */
u8 BSP_I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(); // SDA����Ϊ����
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



























