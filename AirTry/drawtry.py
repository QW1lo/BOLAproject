from math import *
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image, ImageDraw, ImageFont

class GPSVis(object):
    X0 = 0
    Z0 = 0
    def __init__(self, map_path, points, coord_centr):

        self.color_list = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
        self.color_list_TCAS = ['green', 'yellow', 'red', 'black']
        self.points = points
        self.map_path = map_path

        self.centr = coord_centr

        self.result_image = Image
        self.x_ticks = []
        self.y_ticks = []

    def get_ticks(self):
        self.x_ticks = map(
            lambda x: round(x, 4),
            np.linspace(self.points[1], self.points[3], num=7))
        y_ticks = map(
            lambda x: round(x, 4),
            np.linspace(self.points[2], self.points[0], num=8))
        self.y_ticks = sorted(y_ticks, reverse=True)

    def scale_to_img(self, lat_lon, h_w):
        old = (self.points[2], self.points[0])
        new = (0, h_w[1])
        y = ((lat_lon[0] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
        old = (self.points[1], self.points[3])
        new = (0, h_w[0])
        x = ((lat_lon[1] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
        return int(x), h_w[1] - int(y)

    def create_image(self, coord, color, width=2):

        self.result_image = Image.open(self.map_path, 'r')
        draw = ImageDraw.Draw(self.result_image)
        ind_color = 0
        drom1 = (56.1439, 34.9885)
        drom2 = (56.1424, 34.9824)
        x_droml1, y_droml1 = self.scale_to_img((56.0545, 34.6232),
                                               (self.result_image.size[0], self.result_image.size[1]))
        x_droml2, y_droml2 = self.scale_to_img((56.4184, 36.1558),
                                               (self.result_image.size[0], self.result_image.size[1]))
        x_drom, y_drom = self.scale_to_img((56.1439, 34.9885), (self.result_image.size[0], self.result_image.size[1]))
        size = 8
        draw.line([x_droml1, y_droml1, x_droml2, y_droml2], fill='red', width=3)
        draw.ellipse((x_drom - size, y_drom - size, x_drom + size, y_drom + size), fill='red')

        for LA in coord:
            x1, y1 = self.scale_to_img((float(LA[1]), float(LA[2])),
                                       (self.result_image.size[0], self.result_image.size[1]))
            size = 5

            draw.ellipse((x1 - size, y1 - size, x1 + size, y1 + size), fill=self.color_list[ind_color])
            draw.text((x1 - 8, y1 - 8), LA[0], fill='#FFFFFF', font=ImageFont.truetype("arial.ttf", 10))
            ind_color += 1
            if ind_color > 9:
                ind_color = 0


# Пустой желтый фон.
im = Image.new('RGB', (1313, 835))
draw = ImageDraw.Draw(im)

ax1 = plt.figure()

crd_centr = (56.1431, 34.9884) #(55.558738, 37.37884)   # координаты орловки аэродрома
crd_centr = (crd_centr[0]*pi/180, crd_centr[1]*pi/180)
graph = GPSVis(map_path='map2.png', points=(56.4579, 34.4174, 55.8163, 36.2192),
                                    coord_centr=crd_centr)
result_image = graph.result_image = Image.open(graph.map_path, 'r')
ax1.clear()

draw = ImageDraw.Draw(im)
ind_color = 0

x_droml1, y_droml1 = graph.scale_to_img((56.0545, 34.6232), (graph.result_image.size[0], graph.result_image.size[1]))
x_droml2, y_droml2 = graph.scale_to_img((56.4184, 36.1558), (graph.result_image.size[0], graph.result_image.size[1]))
x_drom, y_drom = graph.scale_to_img((56.1439, 34.9885), (graph.result_image.size[0], graph.result_image.size[1]))
size = 8
draw.line([x_droml1, y_droml1, x_droml2, y_droml2], fill='red', width=3)
draw.ellipse((x_drom - size, y_drom - size, x_drom + size, y_drom + size), fill='red')

plt.imshow(im)
plt.imshow(result_image)




# Рисуем красный эллипс с черной оконтовкой.
draw.ellipse((100, 100, 150, 200), fill='red', outline=(0, 0, 0))
plt.show()