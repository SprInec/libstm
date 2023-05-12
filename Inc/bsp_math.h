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

/* Take the maximum of two numbers */
#define max(x, y) ({	\
			typeof(x) _x = (x);	\
			typeof(y) _y = (y);	\
			(void) (&_x == &_y);\
			_x > _y ? _x : _y; })

/* Take the minimum of two numbers */
#define min(x, y) ({	\
			typeof(x) _x = (x);	\
			typeof(y) _y = (y);	\
			(void) (&_x == &_y);\
			_x < _y ? _x : _y; })

/* 自相关函数 */
float bsp_xcorr(float *data, int m, int N);

#endif
