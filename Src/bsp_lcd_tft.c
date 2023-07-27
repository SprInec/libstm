/**
 * @file bsp_lcd.c
 * @author July (Email: JulyCub@163.com)
 * @brief 2.4寸/2.8寸/3.5寸/4.3寸 TFT液晶驱动
 * @version 0.1
 * @date 2023.05.24
 * @addindex 支持驱动IC型号包括:
 * 	ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/
 * 	LGDP4531/LGDP4535/SPFD5408/1505/B505/C505/NT35310/NT35510等
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stdlib.h"
#include "gpio.h"
#include "fsmc.h"
#include "bsp_lcd.h"
#include "bsp_font.h"
#include "bsp_usart.h"
#include "bsp_delay.h"

/* LCD的画笔颜色和背景色 */
u16 POINT_COLOR = 0x0000; // 画笔颜色
u16 BACK_COLOR = 0xFFFF;  // 背景色

/* 管理LCD重要参数,默认为竖屏 */
_lcd_dev lcddev;

// 写寄存器函数
// regval:寄存器值
void LCD_WR_REG(vu16 regval)
{
	regval = regval;	   // 使用-O2优化的时候,必须插入的延时
	LCD->LCD_REG = regval; // 写入要写的寄存器序号
}
// 写LCD数据
// data:要写入的值
void LCD_WR_DATA(vu16 data)
{
	data = data; // 使用-O2优化的时候,必须插入的延时
	LCD->LCD_RAM = data;
}
// 读LCD数据
// 返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram; // 防止被优化
	ram = LCD->LCD_RAM;
	return ram;
}
// 写寄存器
// LCD_Reg:寄存器地址
// LCD_RegValue:要写入的数据
void LCD_WriteReg(vu16 LCD_Reg, vu16 LCD_RegValue)
{
	LCD->LCD_REG = LCD_Reg;		 // 写入要写的寄存器序号
	LCD->LCD_RAM = LCD_RegValue; // 写入数据
}
// 读寄存器
// LCD_Reg:寄存器地址
// 返回值:读到的数据
u16 LCD_ReadReg(vu16 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg); // 写入要读的寄存器序号
	delay_us(5);
	return LCD_RD_DATA(); // 返回读到的值
}
// 开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = lcddev.wramcmd;
}
// LCD写GRAM
// RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD->LCD_RAM = RGB_Code; // 写十六位GRAM
}
// 从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
// 通过该函数转换
// c:GBR格式的颜色值
// 返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16 r, g, b, rgb;
	b = (c >> 0) & 0x1f;
	g = (c >> 5) & 0x3f;
	r = (c >> 11) & 0x1f;
	rgb = (b << 11) + (g << 5) + (r << 0);
	return (rgb);
}
// 当mdk -O1时间优化时需要设置
// 延时i
void opt_delay(u8 i)
{
	while (i--);
}
// 读取个某点的颜色值
// x,y:坐标
// 返回值:此点的颜色
u16 LCD_ReadPoint(u16 x, u16 y)
{
	vu16 r = 0, g = 0, b = 0;
	if (x >= lcddev.width || y >= lcddev.height)
		return 0; // 超过了范围,直接返回
	LCD_SetCursor(x, y);
	if (lcddev.id == 0X9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310)
		LCD_WR_REG(0X2E); // 9341/6804/3510 发送读GRAM指令
	else if (lcddev.id == 0X5510)
		LCD_WR_REG(0X2E00); // 5510 发送读GRAM指令
	else
		LCD_WR_REG(R34); // 其他IC发送读GRAM指令
	if (lcddev.id == 0X9320)
		opt_delay(2); // FOR 9320,延时2us
	LCD_RD_DATA();	  // dummy Read
	opt_delay(2);
	r = LCD_RD_DATA();													   // 实际坐标颜色
	if (lcddev.id == 0X9341 || lcddev.id == 0X5310 || lcddev.id == 0X5510) // 9341/NT35310/NT35510要分2次读出
	{
		opt_delay(2);
		b = LCD_RD_DATA();
		g = r & 0XFF; // 对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
		g <<= 8;
	}
	if (lcddev.id == 0X9325 || lcddev.id == 0X4535 || lcddev.id == 0X4531 || lcddev.id == 0XB505 || lcddev.id == 0XC505)
		return r; // 这几种IC直接返回颜色值
	else if (lcddev.id == 0X9341 || lcddev.id == 0X5310 || lcddev.id == 0X5510)
		return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); // ILI9341/NT35310/NT35510需要公式转换一下
	else
		return LCD_BGR2RGB(r); // 其他IC
}
// LCD开启显示
void LCD_DisplayOn(void)
{
	if (lcddev.id == 0X9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310)
		LCD_WR_REG(0X29); // 开启显示
	else if (lcddev.id == 0X5510)
		LCD_WR_REG(0X2900); // 开启显示
	else
		LCD_WriteReg(R7, 0x0173); // 开启显示
}
// LCD关闭显示
void LCD_DisplayOff(void)
{
	if (lcddev.id == 0X9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310)
		LCD_WR_REG(0X28); // 关闭显示
	else if (lcddev.id == 0X5510)
		LCD_WR_REG(0X2800); // 关闭显示
	else
		LCD_WriteReg(R7, 0x0); // 关闭显示
}
// 设置光标位置
// Xpos:横坐标
// Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if (lcddev.id == 0X9341 || lcddev.id == 0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_DATA(Xpos & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_DATA(Ypos & 0XFF);
	}
	else if (lcddev.id == 0X6804)
	{
		if (lcddev.dir == 1)
			Xpos = lcddev.width - 1 - Xpos; // 横屏时处理
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_DATA(Xpos & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_DATA(Ypos & 0XFF);
	}
	else if (lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_REG(lcddev.setxcmd + 1);
		LCD_WR_DATA(Xpos & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_REG(lcddev.setycmd + 1);
		LCD_WR_DATA(Ypos & 0XFF);
	}
	else
	{
		if (lcddev.dir == 1)
			Xpos = lcddev.width - 1 - Xpos; // 横屏其实就是调转x,y坐标
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}
}
// 设置LCD的自动扫描方向
// 注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
// 所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
// dir:0~7,代表8个方向(具体定义见lcd.h)
// 9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510等IC已经实际测试
void LCD_Scan_Dir(u8 dir)
{
	u16 regval = 0;
	u16 dirreg = 0;
	u16 temp;
	u16 xsize, ysize;
	if (lcddev.dir == 1 && lcddev.id != 0X6804) // 横屏时，对6804不改变扫描方向！
	{
		switch (dir) // 方向转换
		{
		case 0:
			dir = 6;
			break;
		case 1:
			dir = 7;
			break;
		case 2:
			dir = 4;
			break;
		case 3:
			dir = 5;
			break;
		case 4:
			dir = 1;
			break;
		case 5:
			dir = 0;
			break;
		case 6:
			dir = 3;
			break;
		case 7:
			dir = 2;
			break;
		}
	}
	if (lcddev.id == 0x9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310 || lcddev.id == 0X5510) // 9341/6804/5310/5510,很特殊
	{
		switch (dir)
		{
		case L2R_U2D: // 从左到右,从上到下
			regval |= (0 << 7) | (0 << 6) | (0 << 5);
			break;
		case L2R_D2U: // 从左到右,从下到上
			regval |= (1 << 7) | (0 << 6) | (0 << 5);
			break;
		case R2L_U2D: // 从右到左,从上到下
			regval |= (0 << 7) | (1 << 6) | (0 << 5);
			break;
		case R2L_D2U: // 从右到左,从下到上
			regval |= (1 << 7) | (1 << 6) | (0 << 5);
			break;
		case U2D_L2R: // 从上到下,从左到右
			regval |= (0 << 7) | (0 << 6) | (1 << 5);
			break;
		case U2D_R2L: // 从上到下,从右到左
			regval |= (0 << 7) | (1 << 6) | (1 << 5);
			break;
		case D2U_L2R: // 从下到上,从左到右
			regval |= (1 << 7) | (0 << 6) | (1 << 5);
			break;
		case D2U_R2L: // 从下到上,从右到左
			regval |= (1 << 7) | (1 << 6) | (1 << 5);
			break;
		}
		if (lcddev.id == 0X5510)
			dirreg = 0X3600;
		else
			dirreg = 0X36;
		if ((lcddev.id != 0X5310) && (lcddev.id != 0X5510))
			regval |= 0X08; // 5310/5510不需要BGR
		if (lcddev.id == 0X6804)
			regval |= 0x02; // 6804的BIT6和9341的反了
		LCD_WriteReg(dirreg, regval);
		if ((regval & 0X20) || lcddev.dir == 1)
		{
			if (lcddev.width < lcddev.height) // 交换X,Y
			{
				temp = lcddev.width;
				lcddev.width = lcddev.height;
				lcddev.height = temp;
			}
		}
		else
		{
			if (lcddev.width > lcddev.height) // 交换X,Y
			{
				temp = lcddev.width;
				lcddev.width = lcddev.height;
				lcddev.height = temp;
			}
		}
		if (lcddev.dir == 1 && lcddev.id != 0X6804)
		{
			xsize = lcddev.height;
			ysize = lcddev.width;
		}
		else
		{
			xsize = lcddev.width;
			ysize = lcddev.height;
		}
		if (lcddev.id == 0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);
			LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setxcmd + 1);
			LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setxcmd + 2);
			LCD_WR_DATA((xsize - 1) >> 8);
			LCD_WR_REG(lcddev.setxcmd + 3);
			LCD_WR_DATA((xsize - 1) & 0XFF);
			LCD_WR_REG(lcddev.setycmd);
			LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setycmd + 1);
			LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setycmd + 2);
			LCD_WR_DATA((ysize - 1) >> 8);
			LCD_WR_REG(lcddev.setycmd + 3);
			LCD_WR_DATA((ysize - 1) & 0XFF);
		}
		else
		{
			LCD_WR_REG(lcddev.setxcmd);
			LCD_WR_DATA(0);
			LCD_WR_DATA(0);
			LCD_WR_DATA((xsize - 1) >> 8);
			LCD_WR_DATA((xsize - 1) & 0XFF);
			LCD_WR_REG(lcddev.setycmd);
			LCD_WR_DATA(0);
			LCD_WR_DATA(0);
			LCD_WR_DATA((ysize - 1) >> 8);
			LCD_WR_DATA((ysize - 1) & 0XFF);
		}
	}
	else
	{
		switch (dir)
		{
		case L2R_U2D: // 从左到右,从上到下
			regval |= (1 << 5) | (1 << 4) | (0 << 3);
			break;
		case L2R_D2U: // 从左到右,从下到上
			regval |= (0 << 5) | (1 << 4) | (0 << 3);
			break;
		case R2L_U2D: // 从右到左,从上到下
			regval |= (1 << 5) | (0 << 4) | (0 << 3);
			break;
		case R2L_D2U: // 从右到左,从下到上
			regval |= (0 << 5) | (0 << 4) | (0 << 3);
			break;
		case U2D_L2R: // 从上到下,从左到右
			regval |= (1 << 5) | (1 << 4) | (1 << 3);
			break;
		case U2D_R2L: // 从上到下,从右到左
			regval |= (1 << 5) | (0 << 4) | (1 << 3);
			break;
		case D2U_L2R: // 从下到上,从左到右
			regval |= (0 << 5) | (1 << 4) | (1 << 3);
			break;
		case D2U_R2L: // 从下到上,从右到左
			regval |= (0 << 5) | (0 << 4) | (1 << 3);
			break;
		}
		if (lcddev.id == 0x8989) // 8989 IC
		{
			dirreg = 0X11;
			regval |= 0X6040; // 65K
		}
		else // 其他驱动IC
		{
			dirreg = 0X03;
			regval |= 1 << 12;
		}
		LCD_WriteReg(dirreg, regval);
	}
}
// 画点
// x,y:坐标
// POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x, u16 y)
{
	LCD_SetCursor(x, y);	// 设置光标位置
	LCD_WriteRAM_Prepare(); // 开始写入GRAM
	LCD->LCD_RAM = POINT_COLOR;
}
// 快速画点
// x,y:坐标
// color:颜色
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color)
{
	if (lcddev.id == 0X9341 || lcddev.id == 0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x >> 8);
		LCD_WR_DATA(x & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y >> 8);
		LCD_WR_DATA(y & 0XFF);
	}
	else if (lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x >> 8);
		LCD_WR_REG(lcddev.setxcmd + 1);
		LCD_WR_DATA(x & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y >> 8);
		LCD_WR_REG(lcddev.setycmd + 1);
		LCD_WR_DATA(y & 0XFF);
	}
	else if (lcddev.id == 0X6804)
	{
		if (lcddev.dir == 1)
			x = lcddev.width - 1 - x; // 横屏时处理
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x >> 8);
		LCD_WR_DATA(x & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y >> 8);
		LCD_WR_DATA(y & 0XFF);
	}
	else
	{
		if (lcddev.dir == 1)
			x = lcddev.width - 1 - x; // 横屏其实就是调转x,y坐标
		LCD_WriteReg(lcddev.setxcmd, x);
		LCD_WriteReg(lcddev.setycmd, y);
	}
	LCD->LCD_REG = lcddev.wramcmd;
	LCD->LCD_RAM = color;
}

// 设置LCD显示方向
// dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if (dir == 0) // 竖屏
	{
		lcddev.dir = 0; // 竖屏
		lcddev.width = 240;
		lcddev.height = 320;
		if (lcddev.id == 0X9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310)
		{
			lcddev.wramcmd = 0X2C;
			lcddev.setxcmd = 0X2A;
			lcddev.setycmd = 0X2B;
			if (lcddev.id == 0X6804 || lcddev.id == 0X5310)
			{
				lcddev.width = 320;
				lcddev.height = 480;
			}
		}
		else if (lcddev.id == 0x5510)
		{
			lcddev.wramcmd = 0X2C00;
			lcddev.setxcmd = 0X2A00;
			lcddev.setycmd = 0X2B00;
			lcddev.width = 480;
			lcddev.height = 800;
		}
		else
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R32;
			lcddev.setycmd = R33;
		}
	}
	else // 横屏
	{
		lcddev.dir = 1; // 横屏
		lcddev.width = 320;
		lcddev.height = 240;
		if (lcddev.id == 0X9341 || lcddev.id == 0X5310)
		{
			lcddev.wramcmd = 0X2C;
			lcddev.setxcmd = 0X2A;
			lcddev.setycmd = 0X2B;
		}
		else if (lcddev.id == 0X6804)
		{
			lcddev.wramcmd = 0X2C;
			lcddev.setxcmd = 0X2B;
			lcddev.setycmd = 0X2A;
		}
		else if (lcddev.id == 0x5510)
		{
			lcddev.wramcmd = 0X2C00;
			lcddev.setxcmd = 0X2A00;
			lcddev.setycmd = 0X2B00;
			lcddev.width = 800;
			lcddev.height = 480;
		}
		else
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R33;
			lcddev.setycmd = R32;
		}
		if (lcddev.id == 0X6804 || lcddev.id == 0X5310)
		{
			lcddev.width = 480;
			lcddev.height = 320;
		}
	}
	LCD_Scan_Dir(DFT_SCAN_DIR); // 默认扫描方向
}
// 设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
// sx,sy:窗口起始坐标(左上角)
// width,height:窗口宽度和高度,必须大于0!!
// 窗体大小:width*height.
// 68042,横屏时不支持窗口设置!!
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	u8 hsareg, heareg, vsareg, veareg;
	u16 hsaval, heaval, vsaval, veaval;
	width = sx + width - 1;
	height = sy + height - 1;
	if (lcddev.id == 0X9341 || lcddev.id == 0X5310 || lcddev.id == 0X6804) // 6804横屏不支持
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx >> 8);
		LCD_WR_DATA(sx & 0XFF);
		LCD_WR_DATA(width >> 8);
		LCD_WR_DATA(width & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy >> 8);
		LCD_WR_DATA(sy & 0XFF);
		LCD_WR_DATA(height >> 8);
		LCD_WR_DATA(height & 0XFF);
	}
	else if (lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx >> 8);
		LCD_WR_REG(lcddev.setxcmd + 1);
		LCD_WR_DATA(sx & 0XFF);
		LCD_WR_REG(lcddev.setxcmd + 2);
		LCD_WR_DATA(width >> 8);
		LCD_WR_REG(lcddev.setxcmd + 3);
		LCD_WR_DATA(width & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy >> 8);
		LCD_WR_REG(lcddev.setycmd + 1);
		LCD_WR_DATA(sy & 0XFF);
		LCD_WR_REG(lcddev.setycmd + 2);
		LCD_WR_DATA(height >> 8);
		LCD_WR_REG(lcddev.setycmd + 3);
		LCD_WR_DATA(height & 0XFF);
	}
	else // 其他驱动IC
	{
		if (lcddev.dir == 1) // 横屏
		{
			// 窗口值
			hsaval = sy;
			heaval = height;
			vsaval = lcddev.width - width - 1;
			veaval = lcddev.width - sx - 1;
		}
		else
		{
			hsaval = sx;
			heaval = width;
			vsaval = sy;
			veaval = height;
		}
		hsareg = 0X50;
		heareg = 0X51; // 水平方向窗口寄存器
		vsareg = 0X52;
		veareg = 0X53; // 垂直方向窗口寄存器
		// 设置寄存器值
		LCD_WriteReg(hsareg, hsaval);
		LCD_WriteReg(heareg, heaval);
		LCD_WriteReg(vsareg, vsaval);
		LCD_WriteReg(veareg, veaval);
		LCD_SetCursor(sx, sy); // 设置光标位置
	}
}

/**
 * @brief 初始化LCD
 * @attention <!编译器优化级别必须设置为lev-0!>
 * @attention 仅使用于 ILI9341 型号
 * @return SUCCESS-成功|ERROR-失败
 */
void BSP_LCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SRAM_HandleTypeDef hsram;
	FSMC_NORSRAM_TimingTypeDef Timing = {0};
	FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_FSMC_CLK_ENABLE();																				  // 使能FSMC时钟

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5| 
						  GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10| 
						  GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	Timing.AddressSetupTime = 0xF;
	Timing.AddressHoldTime = 0x00;
	Timing.DataSetupTime = 24;
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;

	ExtTiming.AddressSetupTime = 8;
	ExtTiming.AddressHoldTime = 0x00;
	ExtTiming.DataSetupTime = 8;
	ExtTiming.BusTurnAroundDuration = 0x00;
	ExtTiming.CLKDivision = 0x00;
	ExtTiming.DataLatency = 0x00;
	ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

	hsram.Instance = FSMC_NORSRAM_DEVICE;
	hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

	hsram.Init.NSBank = FSMC_NORSRAM_BANK4;
	hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
	hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram.Init.PageSize = FSMC_PAGE_SIZE_NONE;

	if (HAL_SRAM_Init(&hsram, &Timing, &ExtTiming) != HAL_OK)
	{
		Error_Handler();
	}

	delay_ms(50);
	LCD_WriteReg(0x0000, 0x0001);
	delay_ms(50);
	lcddev.id = LCD_ReadReg(0x0000);
	/* 读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300 */
	if (lcddev.id < 0XFF || lcddev.id == 0XFFFF || lcddev.id == 0X9300) 
	{
		LCD_WR_REG(0XD3);
		lcddev.id = LCD_RD_DATA();
		lcddev.id = LCD_RD_DATA();
		lcddev.id = LCD_RD_DATA();
		lcddev.id <<= 8;
		lcddev.id |= LCD_RD_DATA();

		/* 非9341 */
		if (lcddev.id != 0X9341)
		{
			printf("Error ID:%d\n", lcddev.id);
			return;
		}
	}

	/* 9341 初始化 */
	if (lcddev.id == 0X9341) 
	{
		LCD_WR_REG(0xCF);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC1);
		LCD_WR_DATA(0X30);
		LCD_WR_REG(0xED);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0X12);
		LCD_WR_DATA(0X81);
		LCD_WR_REG(0xE8);
		LCD_WR_DATA(0x85);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x7A);
		LCD_WR_REG(0xCB);
		LCD_WR_DATA(0x39);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x02);
		LCD_WR_REG(0xF7);
		LCD_WR_DATA(0x20);
		LCD_WR_REG(0xEA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xC0);  // Power control
		LCD_WR_DATA(0x1B); // VRH[5:0]
		LCD_WR_REG(0xC1);  // Power control
		LCD_WR_DATA(0x01); // SAP[2:0];BT[3:0]
		LCD_WR_REG(0xC5);  // VCM control
		LCD_WR_DATA(0x30); // 3F
		LCD_WR_DATA(0x30); // 3C
		LCD_WR_REG(0xC7);  // VCM control2
		LCD_WR_DATA(0XB7);
		LCD_WR_REG(0x36); // Memory Access Control
		LCD_WR_DATA(0x48);
		LCD_WR_REG(0x3A);
		LCD_WR_DATA(0x55);
		LCD_WR_REG(0xB1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1A);
		LCD_WR_REG(0xB6); // Display Function Control
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0xA2);
		LCD_WR_REG(0xF2); // 3Gamma Function Disable
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0x26); // Gamma curve selected
		LCD_WR_DATA(0x01);
		LCD_WR_REG(0xE0); // Set Gamma
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x2A);
		LCD_WR_DATA(0x28);
		LCD_WR_DATA(0x08);
		LCD_WR_DATA(0x0E);
		LCD_WR_DATA(0x08);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0XA9);
		LCD_WR_DATA(0x43);
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0XE1); // Set Gamma
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x15);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x07);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x2B);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x0F);
		LCD_WR_REG(0x2B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);
		LCD_WR_REG(0x11); // Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); // display on
	}

	/* 选择屏幕显示方式 0-竖屏显示*/
	LCD_Display_Dir(0);
	/* 点亮背光 */
	LCD_LED = 1;
	LCD_Clear(WHITE);
}
// 清屏函数
// color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index = 0;
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height;					// 得到总点数
	if ((lcddev.id == 0X6804) && (lcddev.dir == 1)) // 6804横屏的时候特殊处理
	{
		lcddev.dir = 0;
		lcddev.setxcmd = 0X2A;
		lcddev.setycmd = 0X2B;
		LCD_SetCursor(0x00, 0x0000); // 设置光标位置
		lcddev.dir = 1;
		lcddev.setxcmd = 0X2B;
		lcddev.setycmd = 0X2A;
	}
	else
		LCD_SetCursor(0x00, 0x0000); // 设置光标位置
	LCD_WriteRAM_Prepare();			 // 开始写入GRAM
	for (index = 0; index < totalpoint; index++)
	{
		LCD->LCD_RAM = color;
	}
}

/**
 * @brief 按行清除
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 区域宽度
 * @param height 区域高度
 * @param size 字体大小
 */
void LCD_ClearLine(u16 x, u16 y, u16 width, u16 height, u8 size)
{
	LCD_ShowString(x, y, width, height, size, (uint8_t *)"                                      ");
}

// 在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// color:要填充的颜色
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i, j;
	u16 xlen = 0;
	u16 temp;
	if ((lcddev.id == 0X6804) && (lcddev.dir == 1)) // 6804横屏的时候特殊处理
	{
		temp = sx;
		sx = sy;
		sy = lcddev.width - ex - 1;
		ex = ey;
		ey = lcddev.width - temp - 1;
		lcddev.dir = 0;
		lcddev.setxcmd = 0X2A;
		lcddev.setycmd = 0X2B;
		LCD_Fill(sx, sy, ex, ey, color);
		lcddev.dir = 1;
		lcddev.setxcmd = 0X2B;
		lcddev.setycmd = 0X2A;
	}
	else
	{
		xlen = ex - sx + 1;
		for (i = sy; i <= ey; i++)
		{
			LCD_SetCursor(sx, i);	// 设置光标位置
			LCD_WriteRAM_Prepare(); // 开始写入GRAM
			for (j = 0; j < xlen; j++)
				LCD->LCD_RAM = color; // 显示颜色
		}
	}
}
// 在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// color:要填充的颜色
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
	u16 height, width;
	u16 i, j;
	width = ex - sx + 1;  // 得到填充的宽度
	height = ey - sy + 1; // 高度
	for (i = 0; i < height; i++)
	{
		LCD_SetCursor(sx, sy + i); // 设置光标位置
		LCD_WriteRAM_Prepare();	   // 开始写入GRAM
		for (j = 0; j < width; j++)
			LCD->LCD_RAM = color[i * width + j]; // 写入数据
	}
}
// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; // 计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; // 设置单步方向
	else if (delta_x == 0)
		incx = 0; // 垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; // 水平线
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; // 选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) // 画线输出
	{
		LCD_DrawPoint(uRow, uCol); // 画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
// 画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}
// 在指定位置画一个指定大小的圆
//(x,y):中心点
// r    :半径
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); // 判断下个点位置的标志
	while (a <= b)
	{
		LCD_DrawPoint(x0 + a, y0 - b); // 5
		LCD_DrawPoint(x0 + b, y0 - a); // 0
		LCD_DrawPoint(x0 + b, y0 + a); // 4
		LCD_DrawPoint(x0 + a, y0 + b); // 6
		LCD_DrawPoint(x0 - a, y0 + b); // 1
		LCD_DrawPoint(x0 - b, y0 + a);
		LCD_DrawPoint(x0 - a, y0 - b); // 2
		LCD_DrawPoint(x0 - b, y0 - a); // 7
		a++;
		// 使用Bresenham算法画圆
		if (di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}
// 在指定位置显示一个字符
// x,y:起始坐标
// num:要显示的字符:" "--->"~"
// size:字体大小 12/16/24
// mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
	u8 temp, t1, t;
	u16 y0 = y;
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到字体一个字符对应点阵集所占的字节数
	// 设置窗口
	num = num - ' '; // 得到偏移后的值
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[num][t]; // 调用1206字体
		else if (size == 16)
			temp = asc2_1608[num][t]; // 调用1608字体
		else if (size == 24)
			temp = asc2_2412[num][t]; // 调用2412字体
		else
			return; // 没有的字库
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			else if (mode == 0)
				LCD_Fast_DrawPoint(x, y, BACK_COLOR);
			temp <<= 1;
			y++;
			if (y >= lcddev.height)
				return; // 超区域了
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				if (x >= lcddev.width)
					return; // 超区域了
				break;
			}
		}
	}
}
// m^n函数
// 返回值:m^n次方.
u32 LCD_Pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
// 显示数字,高位为0,则不显示
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// color:颜色
// num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0);
	}
}
// 显示数字,高位为0,还是显示
// x,y:起点坐标
// num:数值(0~999999999);
// len:长度(即要显示的位数)
// size:字体大小
// mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				if (mode & 0X80)
					LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01);
				else
					LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01);
	}
}

/**
 * @brief 显示字符串
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 区域宽度
 * @param height 区域高度
 * @param size 字体大小
 * @param p 字符串地址
 */
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
	u8 x0 = x;
	width += x;
	height += y;
	while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
	{
		if (x >= width)
		{
			x = x0;
			y += size;
		}
		if (y >= height)
			break; // 退出
		LCD_ShowChar(x, y, *p, size, 0);
		x += size / 2;
		p++;
	}
}
