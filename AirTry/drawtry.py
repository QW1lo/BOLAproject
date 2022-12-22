from math import *
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image, ImageDraw, ImageFont

xla = [16004,21707 ]
ppm2 = [-7509, -10165]
ppm1 = [17524, 17948]

dor = []
for i in range(len(ppm1)):
    dor.append(ppm1[i] - ppm2[i])

way = []
for i in range(len(ppm1)):
    way.append(xla[i] - ppm2[i])

plt.plot([0, ppm1[0]], [0, ppm1[1]], color='orange') #ppm1
plt.plot([0, ppm2[0]], [0, ppm2[1]], color='orange') # ppm2
plt.plot([0, xla[0]], [0, xla[1]], color ='blue') # LA

# plt.plot([-10165, 28113 -10165], [-7509, 25033 -7509], color='black') #dor = ppm2 - ppm1
# plt.plot([17948, xla[0] - ppm2[0] +17948], [17524, xla[1] - ppm2[1] +17524], color='red') #way2 =  X-la - ppm2

# plt.plot([0, dor[0]], [0, dor[1]], color='pink')
plt.plot([ppm2[0], dor[0] + ppm2[0]], [ppm2[1], dor[1] + ppm2[1]], color='pink', linewidth=5)
# plt.plot([0, way[0]], [0, way[1] ], color='black')

sin1 = dor[0] * way[1] - way[0] * dor[1]
cos1 = dor[0] * way[0] + dor[1] * way[1]
angle = -atan2(sin1, cos1)
print(angle * 180 / pi)
print(atan2(dor[1], dor[0]) * 180 / pi)
print(atan2(way[1], way[0]) * 180 / pi)

xway = way[0] * cos(angle) - way[1] * sin(angle)
yway = way[0] * sin(angle) + way[1] * cos(angle)
xway_n = xway / sqrt(xway**2 + yway**2)
yway_n = yway / sqrt(xway**2 + yway**2)
xway -= xway_n * 7000
yway -= yway_n * 7000
print('x ',xway , 'y ', yway)
plt.plot([ppm2[0], xway+ppm2[0]], [ppm2[1], yway+ppm2[1]], color='black')
plt.grid()
plt.show()