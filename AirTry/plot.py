import math
import matplotlib.pyplot as plt
data = []
with open("LAoutput.txt") as f:
    for line in f:
        data.append([float(x) for x in line.split()])

T = []
x = []
y = []
z = []

for i in range(len(data)):
    if (len(data[i]) == 4):
        T.append(data[i][0])
        x.append(data[i][1])
        y.append(data[i][2])
        z.append(data[i][3])

ax1 = plt.subplot(1, 2, 1)
ax2 = plt.subplot(1, 2, 2)
ax1.grid()
ax2.grid()


ax1.title.set_text("Oxy")
ax2.title.set_text("Oxz")

ax1.set_xlabel("X, m")
ax1.set_ylabel("Y, m")

ax2.set_xlabel("X, m")
ax2.set_ylabel("Z, m")

ax1.plot(z, y)
ax2.plot(x, z)
ax2.scatter(-2000, -5000, color = 'r')
ax2.scatter(700, 1000, color = 'b')
ax2.scatter(2000, -300, color = 'g')
plt.show()
