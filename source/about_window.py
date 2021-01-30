from PySide6.QtWidgets import QLabel, QWidget, QGridLayout, QLayout
from PySide6.QtCore import Qt


class AboutWindow(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self, parent)

        self.setWindowTitle('About')

        textElement = QLabel(
            "For more information, visit: https://github.com/noobiept/screen_ruler\n\n"
            "You can find there a readme (with the documentation), the source code, and an issues tracker,\n"
            "where you can write suggestions or problems with the application.\n\n"
            "Thanks for using this program.")
        textElement.setTextInteractionFlags(Qt.TextSelectableByMouse
                                            | Qt.TextSelectableByKeyboard)

        layout = QGridLayout()
        layout.addWidget(textElement)
        layout.setSizeConstraint(QLayout.SetFixedSize)

        self.setLayout(layout)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.close()
