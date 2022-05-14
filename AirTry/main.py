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
        crd_centr = (56.1431, 34.9884) #(55.558738, 37.37884)
        crd_centr = (crd_centr[0]*pi/180, crd_centr[1]*pi/180)
        self.graph = risovka.GPSVis(map_path='map.png', points=(56.4336, 34.6014, 55.7920, 36.4032), coord_centr=crd_centr)
        self.timer.start(200)
        threading.Thread(target=self.servak.star_serv).start()


    def update(self):
        self.wdg1.canvas.axes1.clear()
        self.wdg1.canvas.axes2.clear()
        self.wdg1.canvas.axes3.clear()
        self.listH.clear()
        for LA in self.servak.coord:
            self.listH.addItem(LA[0]+' - '+LA[5])
        self.graph.create_image(self.servak.coord, color=(0, 0, 255))
        self.graph.plot_map(self.wdg1.canvas, self.servak.coord)
        self.wdg1.canvas.draw()
        if self.servak.flag_stop:
            self.timer.stop()





app = QApplication([])
window = MatplotlibWidget()
window.show()
app.exec_()
