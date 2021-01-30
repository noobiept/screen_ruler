from PySide6.QtWidgets import QPushButton
from PySide6.QtCore import Qt


class SizeGrip(QPushButton):
    def __init__(self, parent, left):

        super(SizeGrip, self).__init__(parent)

        self.setCursor(Qt.SizeAllCursor)

        self.parent = parent
        self.mouse_being_pressed = False
        self.previous_position = None
        self.length = 20
        self.left = left
        self.setMouseTracking(True)

    def paintEvent(self, event):
        """
            Don't draw anything (transparent)
        """
        pass

    def mousePressEvent(self, event):

        if event.button() == Qt.LeftButton:

            self.mouse_being_pressed = True
            self.previous_position = event.globalPos()

            # don't propagate to the parent
        event.accept()

    def mouseMoveEvent(self, event):

        if self.mouse_being_pressed:
            mousePosition = event.globalPos()

            size = self.parent.size()
            currentWidth = size.width()
            currentHeight = size.height()

            parentPosition = self.parent.pos()
            parentX = parentPosition.x()
            parentY = parentPosition.y()

            if self.left:
                diffX = self.previous_position.x() - mousePosition.x()
                diffY = self.previous_position.y() - mousePosition.y()

                self.parent.move(parentX - diffX, parentY - diffY)
                self.parent.resize(currentWidth + diffX, currentHeight + diffY)

            else:
                diffX = mousePosition.x() - self.previous_position.x()
                diffY = mousePosition.y() - self.previous_position.y()

                self.parent.resize(currentWidth + diffX, currentHeight + diffY)

            self.previous_position = mousePosition

            # propagate the event to the parent widget, but identify we're on top of the SizeGrip (so that we don't move and resize at the same time if we press left click)
            # this is done so that the current length (from the beginning of the ruler to the mouse position) can be shown on the options window, even when the mouse is on top of a SizeGrip
        self.parent.mouseMoveEvent(event, True)

    def mouseReleaseEvent(self, event):

        if event.button() == Qt.LeftButton:
            self.mouse_being_pressed = False

        event.accept()
