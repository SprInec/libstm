/**
 ******************************************************************************
 * @file			: bsp_ad9851.c
 * @brief			: AD9851 Driver.
 * @version		    : 0.1.0
 * @author		    : 康威电子
 * @date			: 2022.07.27
 ******************************************************************************
 * @pinset
 *              PA4     --> CLK
 *              PA5     --> FQ
 *              PA6     --> RST   
 *              PC0     --> D0
 *              PC1     --> D1
 *              PC2     --> D2
 *              PC3     --> D3
 *              PC4     --> D4
 *              PC5     --> D5
 *              PC6     --> D6
 *              PC7     --> D7
 *
 ******************************************************************************
 */

#include "bsp_ad9851.h" 

#ifdef USE_AD9851_SER

uint8_t AD9851_FD=0x01; //AD9851启动6倍频,AD9850不支持倍频，AD9851支持倍频

//***************************************************//
//              AD9851复位(串口模式)                 //
//---------------------------------------------------//
void AD9851_reset_serial()
{
	AD9851_w_clk=0;
	AD9851_fq_up=0;
	//rest信号
	AD9851_rest=0;
	AD9851_rest=1;
	AD9851_rest=0;
	//w_clk信号
	AD9851_w_clk=0;
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//fq_up信号
	AD9851_fq_up=0;
	AD9851_fq_up=1;
	AD9851_fq_up=0;
}

//***************************************************//
//          向AD9851中写命令与数据(串口)             //
//---------------------------------------------------//
void AD9851_wr_serial(uint8_t w0,double frequence)
{
	unsigned char i,w;
	long int y;
	double x;
	//计算频率的HEX值
	x=4294967295/180;//
	
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=frequence;
	//写w4数据
	w=(y>>=0);
	for(i=0;i<8;i++)
	{
		AD9851_bit_data=(w>>i)&0x01;
		AD9851_w_clk=1;
		AD9851_w_clk=0;
	}
	//写w3数据
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		AD9851_bit_data=(w>>i)&0x01;
		AD9851_w_clk=1;
		AD9851_w_clk=0;
	}
	//写w2数据
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		AD9851_bit_data=(w>>i)&0x01;
		AD9851_w_clk=1;
		AD9851_w_clk=0;
	}
	//写w1数据
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		AD9851_bit_data=(w>>i)&0x01;
		AD9851_w_clk=1;
		AD9851_w_clk=0;
	}
	//写w0数据
	w=w0;   
	for(i=0;i<8;i++)
	{
		AD9851_bit_data=(w>>i)&0x01;
		AD9851_w_clk=1;
		AD9851_w_clk=0;
	}
	//移入始能
	AD9851_fq_up=1;
	AD9851_fq_up=0;
}
void AD9851_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ; 

	__HAL_RCC_GPIOA_CLK_ENABLE();	 //使能PA端口时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();	 //使能PA端口时钟

	GPIO_InitStructure.Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 ; 
	GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC ,&GPIO_InitStructure) ;

	GPIO_InitStructure.Pin =  GPIO_Pin_5|GPIO_Pin_4| GPIO_Pin_6; 
	GPIO_Init(GPIOA ,&GPIO_InitStructure) ;
}
//频率输出
void AD9851_Setfq(double fq)
{
	AD9851_wr_serial(AD9851_FD,fq);
}


void AD9851_Init(void)
{
	AD9851_IO_Init();
	D2=0;D1=1;D0=1; //配置串行模式D2到D0需固定电平011，详见芯片数据手册
	AD9851_reset_serial();
}
#endif
#ifdef USE_AD9851_PAR

uint8_t AD9851_FD=0x01; //AD9851启动6倍频,AD9850不支持倍频，AD9851支持倍频

//***************************************************//
//              AD9851复位(并口模式)                 //
//---------------------------------------------------//
void AD9851_reset()
{
	AD9851_w_clk=0;
	AD9851_fq_up=0;
	//rest信号
	AD9851_rest=0;
	AD9851_rest=1;
	AD9851_rest=0;
}

//***************************************************//
//          向AD9851中写命令与数据(并口)             //
//---------------------------------------------------//
void AD9851_wr_parrel(uint8_t w0,double frequence)
{
	uint32_t w;
	long int y;
	double x;
	//计算频率的HEX值
	x=4294967295/180;//适合125M晶振
	//如果时钟频率不为125MHZ，修改该处的频率值，单位MHz ！！！
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=frequence;
	//写w0数据
	w=w0;   
	AD9851_DataBus=w|(w^0xff)<<16;     
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//写w1数据
	w=(y>>24);
	AD9851_DataBus=w|(w^0xff)<<16;     
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//写w2数据
	w=(y>>16);
	AD9851_DataBus=w|(w^0xff)<<16;     
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//写w3数据
	w=(y>>8);
	AD9851_DataBus=w|(w^0xff)<<16;     
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//写w4数据
	w=(y>>=0);
	AD9851_DataBus=w|(w^0xff)<<16;     
	AD9851_w_clk=1;
	AD9851_w_clk=0;
	//移入始能
	AD9851_fq_up=1;
	AD9851_fq_up=0;
}
void AD9851_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC ,&GPIO_InitStructure) ;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4| GPIO_Pin_6; 
	GPIO_Init(GPIOA ,&GPIO_InitStructure) ;
}
//频率输出
void AD9851_Setfq(double fq)
{
	AD9851_wr_parrel(AD9851_FD,fq);
}


void AD9851_Init(void)
{
	AD9851_IO_Init();
	AD9851_reset();
}
#endif






