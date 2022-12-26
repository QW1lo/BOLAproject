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

ax1 = plt.subplot(1,1,1)

v = 150
gammamax = 38

massx1 = [0]
massy1 = [0]
psi = 0
for gamma in range(-gammamax * 10, gammamax * 10, 5):
    gamma = gamma / 10 * pi / 180
    for t in range(35):
        psi += -9.81 / v * tan(gamma)
        massx1.append(massx1[-1] + (v * cos(psi)))
        massy1.append(massy1[-1] + (-v * sin(psi)))
    plt.plot(massx1, massy1)
    psi = 0
    massx1 = [0]
    massy1 = [0]

safezoneback = 1220
safezoneside = 2800
a_el = (v * 35 + safezoneback)
b_el = safezoneside * 2

ellipse = patches.Ellipse(
    xy=(0 + (a_el / 2 - safezoneback) * math.cos(0) , 0 + (a_el / 2 - safezoneback) * math.sin(0)),
    width=a_el,
    height=b_el,
    angle= (0) / math.pi * 180,                    # TODO Рахмер эллипса и поворот
    color='red',
    fill = False
)

ax1.add_patch(ellipse)
ax1.grid()
plt.show()