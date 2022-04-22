import math
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image, ImageDraw


class GPSVis(object):
    def __init__(self, map_path, points, coord_centr):

        self.points = points
        self.map_path = map_path

        self.centr = coord_centr

        self.result_image = Image
        self.x_ticks = []
        self.y_ticks = []

    def plot_map(self, canvas, coord):
        self.get_ticks()
        #fig, axis1 = plt.subplots(figsize=(10, 10))
        canvas.axes1.imshow(self.result_image)
        canvas.axes1.set_xlabel('Longitude')
        canvas.axes1.set_ylabel('Latitude')
        canvas.axes1.set_xticklabels(self.x_ticks)
        canvas.axes1.set_yticklabels(self.y_ticks)
        canvas.axes1.grid()

        canvas.axes2.set_theta_zero_location('N')
        canvas.axes2.set_theta_direction(-1)  
        for LA in coord:
            az, d = self.polar_coord((float(LA[1])*math.pi/180,   float(LA[2])*math.pi/180))
            canvas.axes2.scatter(az, d)


    

    def create_image(self, coord, color, width=2):

        self.result_image = Image.open(self.map_path, 'r')
        draw = ImageDraw.Draw(self.result_image)
        for LA in coord:
                x1, y1 = self.scale_to_img((float(LA[1]),float(LA[2])), (self.result_image.size[0], self.result_image.size[1]))
                size = 5
                draw.ellipse((x1-size,y1-size,x1+size,y1+size), fill=color)


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


