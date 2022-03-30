import math
import matplotlib.pyplot as plt
data = []
datab = []
with open("LAoutput.txt") as f:
    for line in f:
        data.append([float(x) for x in line.split()])

with open("outASP.txt") as f:
    for line in f:
        datab.append([float(x) for x in line.split()])


#LA
T = []
x = []
y = []
z = []

#ASP
Tb = []
xb = []
yb = []
zb = []
vb = []



for i in range(len(datab)):
    if (len(datab[i]) == 7):
        Tb.append(datab[i][0])
        xb.append(datab[i][1])
        yb.append(datab[i][2])
        zb.append(datab[i][3])
        vb.append(datab[i][4])

for i in range(len(data)):
    if (len(data[i]) == 8):
        T.append(data[i][0])
        x.append(data[i][1]) #/ 1000)
        y.append(data[i][2]) #/ 1000)
        z.append(data[i][3]) #/ 1000)

fig1 = plt.figure()
fig2 = plt.figure()

ax1 = fig1.add_subplot(1, 2, 1)
ax2 = fig1.add_subplot(1, 2, 2)
ax1.grid()
ax2.grid()

ax_b1 = fig2.add_subplot(1, 3, 1)
ax_b2 = fig2.add_subplot(1, 3, 2)
ax_b3 = fig2.add_subplot(1, 3, 3)
ax_b1.grid()
ax_b2.grid()

x1 = 1.1 * math.sqrt(2 * 90000 * 9.81 / (0.121 * 100000 * 201))
print(x1)

ax1.title.set_text("Oxy")
ax2.title.set_text("Oxz")

ax1.set_xlabel("X, km")
ax1.set_ylabel("Y, km")

ax2.set_xlabel("X, km")
ax2.set_ylabel("Z, km")

ax_b1.title.set_text("Oxy")
ax_b2.title.set_text("Oxz")

ax_b1.set_xlabel("X, m")
ax_b1.set_ylabel("Y, m")

ax_b2.set_xlabel("X, m")
ax_b2.set_ylabel("Z, m")

#A = abs(math.sqrt(abs(xb[-1]-xb[0])**2 + abs(zb[-1]-zb[0])**2))
#print(A)

ax1.plot(x, y)
ax2.plot(x, z)
ax_b1.plot(xb, yb)
ax_b2.plot(xb, zb)
ax_b3.plot(Tb, vb)

# ax2.scatter(200000  / 1000, -5000 / 1000, color = 'r')
# ax2.scatter(700 / 1000, 1000 / 1000, color = 'b')
# ax2.scatter(2000 / 1000, -300 / 1000, color = 'g')
plt.show()
