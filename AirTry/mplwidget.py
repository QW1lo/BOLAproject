# ------------------------------------------------------
# -------------------- mplwidget.py --------------------
# ------------------------------------------------------
from PyQt5.QtWidgets import *

from matplotlib.backends.backend_qt5agg import FigureCanvas

from matplotlib.figure import Figure
import matplotlib.pyplot

    
class MplWidget(QWidget):
    
    def __init__(self, parent = None):

        QWidget.__init__(self, parent)
        
        self.canvas = FigureCanvas(Figure())
        
        vertical_layout = QVBoxLayout()
        vertical_layout.addWidget(self.canvas)
        
        self.canvas.axes1 = self.canvas.figure.add_subplot(2,2,1)
        self.canvas.axes2 = self.canvas.figure.add_subplot(1,2,2, projection='polar')
        self.canvas.axes3 = self.canvas.figure.add_subplot(2,2,3)
        self.setLayout(vertical_layout)