import math
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
        self.wayLA = [ [[], []], [[], []], [[], []], [[], []], [[], []] ]

    def plot_map(self, canvas, coord):
        # Радар и дальность здесь
        # self.get_ticks()
        # #fig, axis1 = plt.subplots(figsize=(10, 10))
        # canvas.axes1.imshow(self.result_image)
        # canvas.axes1.set_xlabel('Longitude')
        # canvas.axes1.set_ylabel('Latitude')
        # canvas.axes1.set_xticklabels(self.x_ticks)
        # canvas.axes1.set_yticklabels(self.y_ticks)
        # canvas.axes1.grid()
        # self.x_ticks = []
        # self.y_ticks = []

        canvas.axes2.set_theta_zero_location('N')
        canvas.axes2.set_theta_direction(-1)

        canvas.axes3.set_xlabel('Distacnce')
        canvas.axes3.set_ylabel('△Y')
        canvas.axes3.grid()
        for LA in coord:
            d = float(LA[3]) / 1000 #self.polar_coord((float(LA[1])*math.pi/180,   float(LA[2])*math.pi/180))
            az = float(LA[4]) #* 180 / math.pi
            canvas.axes2.scatter(az, d, s=150)
            canvas.axes2.text(az, d, LA[0], horizontalalignment='center', verticalalignment='center', fontdict={'color':'black'}, size=10)
            
            if az<0:
                d = -d
            canvas.axes3.scatter(d, float(LA[6]), s=150)
            canvas.axes3.text(d, float(LA[6]), LA[0], horizontalalignment='center', verticalalignment='center', fontdict={'color':'black'}, size=10)

    def plot2(self, canvas, coord):
        canvas.axes1.grid()
        canvas.axes2.grid()

        for LA in coord:
            if LA[0] == '1':          # Номер ЛА как начало координат
                self.X0 = LA[7]
                self.Z0 = LA[8]
                break


        for LA in coord:
            d = math.sqrt((LA[7] - self.X0)**2 + (LA[8] - self.Z0)**2)
            h = LA[5]
            if (LA[0] == '1'):
                d = 0
                self.X0 = LA[7]
                self.Z0 = LA[8]
            x = LA[7]
            z = LA[8]
            a = (LA[9] * 35)                # длина прм-ка


            pospsi = LA[10]                 # PSI from [-pi;pi] to [0;2pi]
            if LA[10] < 0:
                pospsi += math.pi*2

            anglePsi = LA[10] + math.pi/2
            # if (pospsi  < math.pi/2):
            #     anglePsi = (LA[10] + math.pi / 4 * 3) + math.pi
            # if (pospsi > math.pi and pospsi < math.pi * 3 / 2):
            #     anglePsi = (LA[10] + math.pi / 4 * 3) + math.pi
            # if (pospsi  > math.pi * 3 / 2) or (pospsi > math.pi / 2 and pospsi < math.pi):
            #     anglePsi = (LA[10] - math.pi / 4) + math.pi

            canvas.axes1.scatter(z, x, s=150)

            circle = patches.Circle((z,x), a, color=self.color_list_TCAS[LA[11]], fill = False)

            safezoneback = 1220
            safezoneside = 1220
            a_el = (LA[9] * 35 + safezoneback)
            b_el = safezoneside * 2

            ellipse = patches.Ellipse(
                xy=(z + (a_el / 2 - safezoneside) * math.cos(anglePsi) , x + (a_el / 2 - safezoneside) * math.sin(anglePsi)),
                width=a_el,
                height=b_el,
                angle= (anglePsi) / math.pi * 180,                    # TODO Рахмер эллипса и поворот
                color=self.color_list_TCAS[LA[11]],
                fill = False
            )

            safezoneback = 1220
            safezoneside = 1220
            a_el = (LA[9] * 60 + safezoneback)
            b_el = safezoneside * 2

            ellipsewarning = patches.Ellipse(
                xy=(
                z + (a_el / 2 - safezoneside) * math.cos(anglePsi), x + (a_el / 2 - safezoneside) * math.sin(anglePsi)),
                width=a_el,
                height=b_el,
                angle=(anglePsi) / math.pi * 180,  # TODO Рахмер эллипса и поворот
                color=self.color_list_TCAS[LA[11]],
                fill=False
            )

            #canvas.axes1.scatter(ellipse.get_center()[0], ellipse.get_center()[1], s=150) # показывает центр эллипса
            arrow = patches.Arrow(
                x = z,
                y = x,
                dx = -safezoneback* math.sin(anglePsi),
                dy = -safezoneback* math.cos(anglePsi),
                color= 'pink',
                fill=False
            )

            arrow = patches.Arrow(
                x=z,
                y=x,
                dx=LA[9] * 35 * math.cos(anglePsi),
                dy=LA[9] * 35 * math.sin(anglePsi),
                color=self.color_list_TCAS[LA[11]],
                fill=False
            )

            arrow2 = patches.Arrow(
                x=z,
                y=x,
                dx=LA[13] * 35,
                dy=LA[12] * 35,
                color='black',
                fill=False
            )

            canvas.axes1.add_patch(ellipse)
            canvas.axes1.add_patch(ellipsewarning)
            canvas.axes1.add_patch(arrow)
            canvas.axes1.add_patch(arrow2)


            canvas.axes2.scatter(d, h, s=150)

            rectan = patches.Rectangle(
                    (d - a_el / 2, h - 200 / 2),
                    a_el,
                    200,
                    edgecolor=self.color_list_TCAS[LA[11]],
                    fill=False
            )
            canvas.axes2.add_patch(rectan)


    def plot_map2(self, canvas, coord):
        self.get_ticks()
        #fig, axis1 = plt.subplots(figsize=(10, 10))
        canvas.axes1.imshow(self.result_image)
        canvas.axes1.set_xlabel('Longitude')
        canvas.axes1.set_ylabel('Latitude')
        canvas.axes1.set_xticklabels(self.x_ticks)
        canvas.axes1.set_yticklabels(self.y_ticks)
        canvas.axes1.grid()
        for LA in coord:
            canvas.axes1.plot(self.wayLA[int(LA[0]) - 1][0], self.wayLA[int(LA[0]) - 1][1])
        self.x_ticks = []
        self.y_ticks = []
    

    def create_image(self, coord, color, width=2):

        self.result_image = Image.open(self.map_path, 'r')
        draw = ImageDraw.Draw(self.result_image)
        ind_color = 0
        drom1 = (56.1439, 34.9885)
        drom2 = (56.1424, 34.9824)
        x_droml11, y_droml11 = self.scale_to_img((56.077251, 34.8289), (self.result_image.size[0], self.result_image.size[1]))
        x_droml12, y_droml12 = self.scale_to_img((56.302061, 35.283497), (self.result_image.size[0], self.result_image.size[1]))

        x_droml21, y_droml21 = self.scale_to_img((56.08095, 35.3224956),
                                                 (self.result_image.size[0], self.result_image.size[1]))
        x_droml22, y_droml22 = self.scale_to_img((56.31982, 34.748876),
                                                 (self.result_image.size[0], self.result_image.size[1]))

        x_drom, y_drom = self.scale_to_img((56.1439, 34.9885), (self.result_image.size[0], self.result_image.size[1]))
        size = 8
        draw.line([x_droml11, y_droml11, x_droml12, y_droml12], fill='red', width=3)
        draw.line([x_droml21, y_droml21, x_droml22, y_droml22], fill='red', width=3)
        draw.ellipse((x_drom-size,y_drom-size,x_drom+size,y_drom+size), fill='red')
        
        for LA in coord:
                x1, y1 = self.scale_to_img((float(LA[1]),float(LA[2])), (self.result_image.size[0], self.result_image.size[1]))
                size = 5
                if (x1 > 0) and (y1 > 0):
                    self.wayLA[int(LA[0]) - 1][0].append(x1)
                    self.wayLA[int(LA[0]) - 1][1].append(y1)
                draw.ellipse((x1-size,y1-size,x1+size,y1+size), fill=self.color_list[ind_color])
                draw.text((x1-8,y1-8),LA[0],fill='#FFFFFF',font=ImageFont.truetype("arial.ttf", 10))
                ind_color += 1
                if ind_color > 9:
                    ind_color = 0
                


    def scale_to_img(self, lat_lon, h_w):
        old = (self.points[2], self.points[0])
        new = (0, h_w[1])
        y = ((lat_lon[0] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
        old = (self.points[1], self.points[3])
        new = (0, h_w[0])
        x = ((lat_lon[1] - old[0]) * (new[1] - new[0]) / (old[1] - old[0])) + new[0]
        return int(x), h_w[1] - int(y)

    def get_ticks(self):
        self.x_ticks = map(
            lambda x: round(x, 4),
            np.linspace(self.points[1], self.points[3], num=7))
        y_ticks = map(
            lambda x: round(x, 4),
            np.linspace(self.points[2], self.points[0], num=8))
        self.y_ticks = sorted(y_ticks, reverse=True)
    
    def polar_coord(self, coord):
        #pi - число pi, rad - радиус сферы (Земли)
        rad = 6372795

        
        #в радианах
        lat1 = self.centr[0]
        lat2 = coord[0]
        long1 = self.centr[1]
        long2 = coord[1]
        
        #косинусы и синусы широт и разницы долгот
        cl1 = math.cos(lat1)
        cl2 = math.cos(lat2)
        sl1 = math.sin(lat1)
        sl2 = math.sin(lat2)
        delta = long2 - long1
        cdelta = math.cos(delta)
        sdelta = math.sin(delta)
        
        #вычисления длины большого круга
        y = math.sqrt(math.pow(cl2*sdelta,2)+math.pow(cl1*sl2-sl1*cl2*cdelta,2))
        x = sl1*sl2+cl1*cl2*cdelta
        ad = math.atan2(y,x)
        dist = ad*rad
        
        #вычисление начального азимута
        x = (cl1*sl2) - (sl1*cl2*cdelta)
        y = sdelta*cl2
        z = math.degrees(math.atan(-y/x))
        
        if (x < 0):
            z = z+180.
        
        z2 = (z+180.) % 360. - 180.
        z2 = - math.radians(z2)
        anglerad2 = z2 - ((2*math.pi)*math.floor((z2/(2*math.pi))) )
        angledeg = (anglerad2*180.)/math.pi
        return anglerad2, dist/1000

    # def delta_lmd(self, lmd1, lmd2):
    #     if abs(lmd2-lmd1)<=math.pi:
    #         delta = lmd2-lmd1
    #     if(lmd2-lmd1<-math.pi):
    #         delta = 2*math.pi + lmd2-lmd1
    #     if(lmd2-lmd1>math.pi):
    #         delta = lmd2-lmd1-2*math.pi
    #     return delta

    # def azimut(self, coord):
    #     phi1 = self.centr[0]
    #     lmd1 = self.centr[1]

    #     phi2 = coord[0]
    #     lmd2 = coord[1]

    #     a = 6378137
    #     b = 6356752.3142 

    #     e_2 = 1 - (b**2)/(a**2)
    #     e1 = e_2**0.5  #2.718281828459045 #exp #
    #     a = math.log(math.tan(math.pi/4 + phi2/2)* (  (1-e1*math.sin(phi2))/(1+e1*math.sin(phi2)))**(e1/2))
    #     b = math.log(math.tan(math.pi/4 + phi1/2)* (  (1-e1*math.sin(phi1))/(1+e1*math.sin(phi1)))**(e1/2))
        
    #     az = math.atan(math.delta_lmd(lmd1, lmd2)/(a-b))
    #     return az

    # def dalnost(self, coord):
    #     phi1 = self.centr[0]
    #     lmd1 = self.centr[1]

    #     phi2 = coord[0]
    #     lmd2 = coord[1]
    #     az = self.azimut(coord)
    #     a = 6378137
    #     b = 6356752.3142 

    #     e_2 = 1 - (b**2)/(a**2)

    #     d = (a/math.cos(az)) * (  (1-e_2/4)*(phi2-phi1) -  3*e_2*(math.sin(2*phi2)-math.sin(2*phi1))/8  ) 
    #     return az, d


print(plt.rcParams['axes.prop_cycle'].by_key()['color'])
