import matplotlib.pyplot as plt
import numpy as np
from PIL import Image, ImageDraw
from matplotlib.animation import FuncAnimation


data_path = 'gps.txt'
points = (56.4336, 34.6014, 55.7920, 36.4032)
#points = (56.2617, 34.7003, 56.0195, 35.3430)
map_path = 'map.png'
#map_path = 'map2.png'
result_image = Image


fig, axis1 = plt.subplots(figsize=(10, 10))

def get_ticks():
    x_ticks = map(
        lambda x: round(x, 4),
        np.linspace(points[1], points[3], num=7))
    y_ticks = map(
        lambda x: round(x, 4),
        np.linspace(points[2], points[0], num=8))
    y_ticks = sorted(y_ticks, reverse=True)
    return x_ticks, y_ticks


def plot_map():
    x_ticks, y_ticks = get_ticks()
    #result_image = Image.open(map_path, 'r')
    global result_image
    axis1.imshow(result_image)
    axis1.set_xlabel('Longitude')
    axis1.set_ylabel('Latitude')
    axis1.set_xticklabels(x_ticks)
    axis1.set_yticklabels(y_ticks)
    axis1.grid()
    #plt.show()

def create_image(coord, color, width=2):
    global result_image
    result_image = Image.open(map_path, 'r')
    draw = ImageDraw.Draw(result_image)
    for LA in coord:
            x1, y1 = scale_to_img((float(LA[1]),float(LA[2])), (result_image.size[0], result_image.size[1]))
            draw.ellipse((x1-2,y1-2,x1+2,y1+2), fill=color)



def scale_to_img(lat_lon, h_w):
    old = (points[2], points[0])
    new = (0, h_w[1])
    y = ((lat_lon[0] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
    old = (points[1], points[3])
    new = (0, h_w[0])
    x = ((lat_lon[1] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
    return int(x), h_w[1] - int(y)

def ris(coord):
    def animate(i):
        plt.cla()
        create_image(coord, color=(0, 0, 255),width=3)
        plot_map()


    ani = FuncAnimation(fig, animate, interval=1)

    plt.tight_layout()
    plt.show()
