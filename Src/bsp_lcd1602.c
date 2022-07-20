/*
 ******************************************************************************
 * @file		:bsp_lcd1602.c
 * @brief		:The board support package for LCD1602.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.28
 ******************************************************************************
 */
 
#include "bsp_lcd1602.h"


void lcd_delay1ms(unsigned char d)   //误差 0us
{
    unsigned char a,b,c;
    for(c=0;c<d;c++)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}

void lcd_Init(){
	lcd_write_com(0x38);
	lcd_delay1ms(1);
	lcd_write_com(0x38);	//设置显示模式
	lcd_write_com(0x0c);	//开显示不显示贯标
	lcd_write_com(0x06);	//指针自动加一
	lcd_write_com(0x01);	//清屏
	lcd_write_com(0x80);	//设置数据指针起点
}

void lcd_write_com(unsigned char com){
	RS(0);
	RW(0);
	E(0);
	
	LCD1602_Port2->ODR |= com;
	lcd_delay1ms(10);
	E(1);
	lcd_delay1ms(10);
	E(0);
}

void lcd_write_dat(unsigned char dat){
	RS(1);
	RW(0);
	E(0);
	
	LCD1602_Port2->ODR |= dat;
	lcd_delay1ms(10);
	E(1);
	lcd_delay1ms(10);
	E(0); 
}


void lcd_xy(unsigned char x,unsigned char y){
	lcd_write_com(x+y);
}

void lcd_read_busy(){
	unsigned char he,a;
	RS(0);
	RW(1);
	E(1);
	he = LCD1602_Port2->IDR;
	while(1){
		he = LCD1602_Port2->IDR;
		E(0);
		if(!(he&0x80))//当he&0x80为0时跳出循环，表明不忙。
			break;
		E(1);
		a++;
		if(a>=10)
			break;
	}
}
