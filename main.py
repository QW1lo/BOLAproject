import matplotlib.pyplot as plt
import numpy as np
from shapely.geometry.polygon import LinearRing
from math import *

def ellipse_polyline(ellipses, n=100):
    t = np.linspace(0, 2*np.pi, n, endpoint=False)
    st = np.sin(t)
    ct = np.cos(t)
    result = []
    for x0, y0, a, b, angle in ellipses:
        angle = np.deg2rad(angle)
        sa = np.sin(angle)
        ca = np.cos(angle)
        p = np.empty((n, 2))
        p[:, 0] = x0 + a * ca * ct - b * sa * st
        p[:, 1] = y0 + a * sa * ct + b * ca * st
        result.append(p)
    return result

def intersections(a, b):
    ea = LinearRing(a)
    eb = LinearRing(b)
    mp = ea.intersection(eb)

    x = [p.x for p in mp.geoms]
    y = [p.y for p in mp.geoms]
    return x, y

ellipses = [(0, -1, 2, 1, 0), (2, 0.5, 5, 1.5, 0)]
a, b = ellipse_polyline(ellipses)
x, y = intersections(a, b)
# plt.plot(x, y, "o")
# plt.plot(a[:,0], a[:,1])
# plt.plot(b[:,0], b[:,1])


y1 = -1
y2 = 0.5
x1 = 0
x2 = 2

w1 = 2
w2 = 5
h1 = 1
h2 = 1.5

tangens = (y2-y1)/(x2-x1)
cosinus = sqrt( 1 / (tangens*tangens + 1))
sinus = sqrt( 1 - cosinus*cosinus )
radius1 = abs(w1-x1)*abs(h1-y1) / sqrt( (h1-y1)*(h1-y1)*cosinus*cosinus + (w1-x1)*(w1-x1)*sinus*sinus )
radius2 = abs(w2-x2)*abs(h2-y2) / sqrt( (h2-y2)*(h2-y2)*cosinus*cosinus + (w2-x2)*(w2-x2)*sinus*sinus )

rasstoyanie = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))

if (rasstoyanie <= radius2 + radius1):
    print('пересекаются')
else:
    print('не пересекаются')

xm = [0]
ym = [0]
xc = []
yc = []
v = 25
om = 6.8 * pi/180
for i in range(45):
    angle = 0 + i * om
    xm.append(xm[-1] + (v * cos(angle)))
    ym.append(ym[-1] +(v * sin(angle)))

for i in range(len(xm)):
    #xm[i] += xm[0]
    pass
# xm.append(0)
# ym.append(0)
# v = -100 * 45
# for i in range(45):
#     angle = 0 + i * om
#     xm.append((v * cos(angle))+xm[-1])
#     ym.append(v * sin(angle)+ym[-1])
#
# xm.append(0)
# ym.append(0)
# v = 100 * 45
# for i in range(45):
#     angle = 0 + i * om
#     xm.append((v * sin(angle))+xm[-1])
#     ym.append(v * cos(angle)+ym[-1])
#
# xm.append(0)
# ym.append(0)
# v = -100 * 45
# for i in range(45):
#     angle = 0 + i * om
#     xm.append((v * sin(angle))+xm[-1])
#     ym.append(v * cos(angle)+ym[-1])

print(xm)
print(ym)
plt.plot(xm, ym)
plt.grid()
plt.show()
