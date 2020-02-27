from PySide2.QtWidgets import QWidget, QTabWidget, QVBoxLayout
from PySide2.QtCore import Qt

from options_colors_tab import OptionsColorsTab
from options_general_tab import OptionsGeneralTab


class OptionsWindow(QWidget):
    def __init__(self, ruler, parent=None):
        QWidget.__init__(self, parent)

        self.ruler = ruler
        self.generalTab = OptionsGeneralTab(ruler)
        self.colorsTab = OptionsColorsTab(ruler)

        self.tabWidget = QTabWidget()
        self.tabWidget.addTab(self.generalTab, "General")
        self.tabWidget.addTab(self.colorsTab, "Colors")

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.tabWidget)
        self.setLayout(mainLayout)
        self.setWindowTitle("Options")
        self.show()

    def closeEvent(self, event):
        self.hide()
        event.ignore()

    def setCurrentLength(self, length):
        if self.isVisible():
            self.generalTab.setCurrentLength(length)

    def updateOrientation(self, isHorizontal):
        if self.isVisible():
            self.generalTab.updateOrientation(isHorizontal)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.close()
