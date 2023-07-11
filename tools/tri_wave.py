# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as  plt

# 点数
POINTS = 36
# 最大值
MAX = 43700
# 最小值
MIN = 20700

# 三角波生成器
# points 点数
# min 最小值
# max 最大值
def Triangle_wave(points, min, max):
    x = np.arange(0, points)
    y = []
    gap = ((max - min) / (points / 2))
    temp = min
    while temp < max:
        y.append(int(temp))
        temp += gap
    while temp > min:
        y.append(int(temp))
        temp -= gap
    print(f"x = {x}")
    print(f"y = {y}")
    return x, y
if __name__ == '__main__':
    x, y = Triangle_wave(POINTS, MIN, MAX)
    plt.figure()
    plt.plot(x, y)
    plt.show()



