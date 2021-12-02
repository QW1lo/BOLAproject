import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

data = []
with open("LAoutput.txt") as f:
    for line in f:
        data.append([float(x) for x in line.split()])

T = []
x = []
y = []
z = []

for i in range(len(data)):
    if (len(data[i]) == 5):
        T.append(data[i][0])
        x.append(data[i][1])
        y.append(data[i][2])
        z.append(data[i][3])

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.plot(x, z, y)  # Z и Y поменяны местами для удобного отображения
#ax.scatter(100000, 0, 14000, color = 'r')

ax.set_xlabel("X, m")
ax.set_ylabel("Z, m")
ax.set_zlabel('Y, m')

plt.show()


#ax.scatter(-2000, -5000, color = 'r')
#ax.scatter(700, 1000, color = 'b')
#ax.scatter(2000, -300, color = 'g')

