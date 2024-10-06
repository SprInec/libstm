# -*- coding: utf-8 -*-

"""
    need packages:
        - numoy
        - matplotlib
        - math
"""
import numpy as np
import matplotlib.pyplot as plt
import math

fig = plt.figure(figsize=(15,15))
ax = fig.add_subplot(1, 1, 1)

# 半波分段数
hwave_segment = 32

# 调制深度
div = 0.7

# PWM定时器 ARR
arr = 1000

# 占空比序列
duty = []

# 占空比寄存器值序列
duty_reg = []

# 生成正弦波
sin_x = np.linspace(0, math.pi, hwave_segment + 1)
sin_y = np.sin(sin_x)

if __name__ == '__main__':
    for i in range(hwave_segment):
        sin_area = np.cos(sin_x[i]) - np.cos(sin_x[i + 1])
        single_duty = (sin_area * div) / sin_x[1]
        pwm = sin_x[i] + single_duty * sin_x[i]
        duty.append(single_duty)
        duty_reg.append(round(single_duty * arr))

    print(f"{duty}\r{duty_reg}")
    picl = plt.plot(sin_x, sin_y)
    plt.show()