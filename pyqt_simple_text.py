from PyQt5.QtWidgets import (QApplication,QVBoxLayout,QWidget,QGroupBox,
                             QFormLayout,QLineEdit,QPushButton)
import sys
import requests

class SecWindow(QWidget):

    def __init__(self, parent=None):
        super(SecWindow, self).__init__(parent)

        self.gain_send=0
        self.height_send=0
        self.width_send=0
        self.exposure_time_send=0
        self.show_second_window()

    def show_second_window(self):
        outer = QGroupBox("Change the  camera parameter values")
        form_for_camera_parameters = QFormLayout()

        self.gain = QLineEdit()
        self.gain.textChanged.connect(self.set_gain_value)
            
        self.height_=QLineEdit()
        self.height_.textChanged.connect(self.set_height_value)

        self.width_=QLineEdit()
        self.width_.textChanged.connect(self.set_width_value)

        self.exposure_time=QLineEdit()
        self.exposure_time.textChanged.connect(self.set_exposure_value)

        form_for_camera_parameters.addRow("gain", self.gain)
        form_for_camera_parameters.addRow("height", self.height_)
        form_for_camera_parameters.addRow("width", self.width_)
        form_for_camera_parameters.addRow("exposure_time", self.exposure_time)

        outer.setLayout(form_for_camera_parameters)

        save=QPushButton("Save")
        save.clicked.connect(self.on_save_button_parameters)

        params_ver = QVBoxLayout()

        params_ver.addWidget(outer)
        params_ver.addWidget(save)

        self.setLayout(params_ver)

    def set_gain_value(self):
        self.gain_send=self.gain.text()
        print(self.gain_send)

    def set_height_value(self):
        self.height_send=self.height_.text()
        print(self.height_send)

    def set_width_value(self):
        self.width_send=self.width_.text()
        print(self.width_send)

    def set_exposure_value(self):
        self.exposure_time_send=self.exposure_time.text()
        print(self.exposure_time_send)

    def on_save_button_parameters(self):
        print(int(self.height_send),int(self.width_send),float(self.gain_send))
        self.hide()

        


if __name__=="__main__":
    app = QApplication(sys.argv)
    window = SecWindow()
    window.show()
    sys.exit(app.exec())
