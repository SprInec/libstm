/**
 ******************************************************************************
 * @file			:bsp_math.h
 * @brief			:some math function.
 * @version			:0.1.0
 * @author			:July
 * @date			:2022.07.05
 ******************************************************************************
 */

#ifndef __BSP_MATH_H__
#define __BSP_MATH_H__

#include "bsp_config.h"
#include "math.h"

#define PI acos(-1)

/* Take the maximum of two numbers */
#define __bsp_max(x, y) ({	\
			typeof(x) _x = (x);	\
			typeof(y) _y = (y);	\
			(void) (&_x == &_y);\
			_x > _y ? _x : _y; })

/* Take the minimum of two numbers */
#define __bsp_min(x, y) ({	\
			typeof(x) _x = (x);	\
			typeof(y) _y = (y);	\
			(void) (&_x == &_y);\
			_x < _y ? _x : _y; })

/* 正弦码表生成器 */
void bsp_sin(uint16_t points, float *result);
/* 自相关函数 */
float bsp_xcorr(float *data, int m, int N);

#endif
