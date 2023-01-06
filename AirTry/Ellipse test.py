import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from shapely.geometry.polygon import LinearRing
from math import *

import numpy as np
from scipy.linalg import eigh
from scipy.optimize import minimize_scalar
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse

m = [ [[],[]], [[]], [[]] ]
m[0][0].append(3)
m[0][1].append(1)
print(m)

a01 = 22.360679774997896964091736687313
b01 = 10
theta01 = 45

a02 = 6.41
b02 = 6.41

tmp1 = tan(theta01 / 180 * pi)

def solvew(a,b,theta):
    tmp = tan(theta)
    w = []
    w.append( sqrt((a/2)**2 / (tmp**2 + 1)))
    w.append(w[0] * tmp)
    h = b/a
    return w, h

def solvew1(a,b,theta):
    tmp = tan(theta)
    w = []
    w.append( a/2 * cos(theta))
    w.append(a/2 * sin(theta))
    h = b/a
    return w, h

w3, h3 = solvew(a01, b01, theta01)

ax1 = plt.subplot(1,1,1)

w1 = [5538.8893969814308,-285.6397796208653]
h1 = 0.37714755381604695
w2 = [14352,12520]
h2 = 0.10981

w4 = [100, 0]
print(atan2(w1[1],w1[0]))
print(tan(63.43494882292201 / 180 * pi))

w1,h1 = solvew1(11091.428571428572, 4182.8571428571431,3.0900380367025195)
w2,h2 = solvew1(38091.428571428572, 4182.8571428571431,0.71732623130891504)

a1 = 2* sqrt(w1[0]**2 + w1[1]**2)
b1 = 2* sqrt(w1[0]**2 + w1[1]**2) * h1

a2 = 2 * sqrt(w2[0] ** 2 + w2[1] ** 2)
b2 = 2 * sqrt(w2[0] ** 2 + w2[1] ** 2) * h2

a3 = 2 * sqrt(w3[0] ** 2 + w3[1] ** 2)
b3 = 2 * sqrt(w3[0] ** 2 + w3[1] ** 2) * h3

ellipse = patches.Ellipse(
                xy=(16217, 4575),
                width=a1,
                height=b1,
                angle= math.atan2(w1[1],w1[0]) * 180 / math.pi,
                fill = False
            )
ellipse2 = patches.Ellipse(
                xy=(15813, 15576),
                width=a2,
                height=b2,
                angle = math.atan2(w2[1],w2[0]) * 180 / math.pi,
                color= 'red',
                fill = False
            )
print('ATAN = ', math.atan2(w2[1],w2[0]))
angle=math.atan2(w3[1], w3[0]) * 180 / math.pi
ellipse3 = patches.Ellipse(
                xy=(0, 0),
                width=500,
                height=1000,
                angle=math.atan2(w4[1], w4[0]) * 180 / math.pi,
                color= 'orange',
                fill=False
                        )

x1 = -4469.881
z1 = -15315.85921

v = 101.700493
safezoneback = 1220
safezoneside = 1220
a = (v * 35 + safezoneback)
b = safezoneside * 2
xy = [-4469.881, -15315.85921]
w = 20 * pi / 180


PSI =-0.599283
print('solve0',solvew(a, b, PSI + pi/2))

v2 = 100.587
a2 = (v2 * 35 + safezoneback)
b2 = safezoneside * 2
xy2 = [-4689.14096, -19408.799983]
PSI2 = 0.037608
anglePsi2 = PSI2 + pi/2
print('solve2',solvew(a2, b2, PSI2 + pi/2))

print('atana', atan2(100.218518, 4.186619))
pospsi = PSI
if PSI < 0:
    pospsi += math.pi*2 + pi/2
anglePsi = PSI + pi/2
# if (pospsi < math.pi / 2):
#     anglePsi = (PSI + math.pi / 4 * 3) + math.pi
# if (pospsi > math.pi and pospsi < math.pi * 3 / 2):
#     anglePsi = (PSI + math.pi / 4 * 3) + math.pi
# if (pospsi > math.pi * 3 / 2) or (pospsi > math.pi / 2 and pospsi < math.pi):
#     anglePsi = (PSI - math.pi / 4) + math.pi

#anglePsi -= 1.170334 + pi

print('delangle', anglePsi * 180/ pi)
# Эллипс для ЛА раюотает!
# ellipseLA = patches.Ellipse(
#                 xy=(xy[0] + (a/2 - safezoneside) * cos(w), xy[1] + (a/2 - safezoneside)* sin(w)),
#                 width=a,
#                 height=b,
#                 angle=w * 180/ pi,
#                 color= 'orange',
#                 fill=False
#                         )
ellipseLA = patches.Ellipse(
                xy= (xy[1] + (a/2 - safezoneside) * cos(anglePsi), xy[0] + (a/2 - safezoneside) * sin(anglePsi)),
                width=a,
                height=b,
                angle=(anglePsi) * 180/ pi,
                color= 'orange',
                fill=False
                        )

ellipseLA2 = patches.Ellipse(
                xy= (xy2[1] + (a2/2 - safezoneside) * cos(anglePsi2), xy2[0] + (a2/2 - safezoneside) * sin(anglePsi2)),
                width=a2,
                height=b2,
                angle=(anglePsi2) * 180/ pi,
                color= 'orange',
                fill=False
                        )
arrow = patches.Arrow(
                x = xy[1],
                y = xy[0],
                dx = v * 35 * math.cos(anglePsi),
                dy = v * 35 * math.sin(anglePsi),
                color= 'orange',
                fill=False
            )

arrow2 = patches.Arrow(
                x = xy[1],
                y = xy[0],
                dx = -1220 * math.cos(anglePsi),
                dy = -1220 * math.sin(anglePsi),
                color= 'red',
                fill=False
            )

arrow_v = patches.Arrow(
                x = xy[1],
                y = xy[0],
                dx = 60.2103 * 35,
                dy = 81.189726 * 35,
                color= 'black',
                fill=False
            )

arrow_right = patches.Arrow(
                x = xy[1] + (a/2 - safezoneside) * cos(anglePsi),
                y = xy[0] + (a/2 - safezoneside) * sin(anglePsi),
                dx = -1220 * math.cos(anglePsi + pi/2),
                dy = -1220 * math.sin(anglePsi + pi/2),
                color= 'blue',
                fill=False
            )

arrow_v2 = patches.Arrow(
                x = xy2[1],
                y = xy2[0],
                dx = -5.68 * 35,
                dy = 96.99 * 35,
                color= 'black',
                fill=False
            )
arrow22 = patches.Arrow(
                x = xy2[1],
                y = xy2[0],
                dx = v2 * 35 * math.cos(anglePsi2),
                dy = v2 * 35 * math.sin(anglePsi2),
                color= 'orange',
                fill=False
            )
ax1.add_patch(ellipse)
ax1.add_patch(ellipse2)
ax1.add_patch(ellipse3)
# ax1.add_patch(ellipseLA)
# ax1.add_patch(ellipseLA2)
# ax1.add_patch(arrow)
# ax1.add_patch(arrow2)
# ax1.add_patch(arrow_v)
# ax1.add_patch(arrow_v2)
# ax1.add_patch(arrow22)
# ax1.add_patch(arrow_right)
ax1.scatter([xy[1],ellipseLA.get_center()[0]], [xy[0],ellipseLA.get_center()[1]])
#ax1.plot([0,ellipseLA.get_center()[0]], [0,ellipseLA.get_center()[1]])
ax1.grid()

print(10* log10(2*10**4 + 10**4.5 + 10**4.5))

plt.show()