from PyQt5.QtWidgets import*
from math import pi
from PyQt5.uic import loadUi
from matplotlib.backends.backend_qt5agg import (NavigationToolbar2QT as NavigationToolbar)


from PyQt5.QtCore import QTimer
import threading

import Server
import risovka


     
class MatplotlibWidget(QMainWindow):
    
    def __init__(self):
        
        QMainWindow.__init__(self)

        loadUi("form.ui", self)

        self.setWindowTitle("RADAR")
        self.addToolBar(NavigationToolbar(self.wdg1.canvas, self))

        self.timer = QTimer(self)                               
        self.timer.timeout.connect(self.update)

        self.servak = Server.serv()
        crd_centr = (56.1431, 34.9884) #(55.558738, 37.37884)   # координаты орловки аэродрома
        crd_centr = (crd_centr[0]*pi/180, crd_centr[1]*pi/180)
        #self.graph = risovka.GPSVis(map_path='map.png', points=(56.4336, 34.6014, 55.7920, 36.4032), coord_centr=crd_centr)
        self.graph = risovka.GPSVis(map_path='map2.png', points=(56.4579, 34.4174, 55.8163, 36.2192),
                                    coord_centr=crd_centr)
        self.timer.start(200)
        threading.Thread(target=self.servak.star_serv).start()


    def update(self):
        #self.wdg1.canvas.axes1.clear()
        self.wdg1.canvas.axes2.clear()
        self.wdg1.canvas.axes3.clear()

        self.wdg2.canvas.axes1.clear()
        self.wdg2.canvas.axes2.clear()

        self.wdg3.canvas.axes1.clear()
        # self.wdg2.canvas.axes1.scatter(0, 0, s=150)
        # self.wdg2.canvas.axes1.set_xlabel('Longitude')
        # self.wdg2.canvas.axes1.set_ylabel('Latitude')
        # self.wdg2.canvas.axes1.grid()
        # self.wdg2.canvas.draw()
        self.listH.clear()
        for LA in self.servak.coord:
            self.listH.addItem(LA[0]+' - '+str(LA[5]))
        self.graph.create_image(self.servak.coord, color=(0, 0, 255))
        self.graph.plot_map(self.wdg1.canvas, self.servak.coord)
        self.graph.plot2(self.wdg2.canvas, self.servak.coord)
        self.graph.plot_map2(self.wdg3.canvas, self.servak.coord)

        self.wdg1.canvas.draw()
        self.wdg2.canvas.draw()
        self.wdg3.canvas.draw()
        if self.servak.flag_stop:
            self.timer.stop()





app = QApplication([])
window = MatplotlibWidget()
window.show()
app.exec_()
