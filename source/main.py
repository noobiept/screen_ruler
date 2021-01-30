#!/usr/bin/env python3
import sys

from PySide6.QtWidgets import QApplication, QWidget, QMenu, QStyle
from PySide6.QtGui import QPainter, QFont, QFontMetrics, QCursor, QAction
from PySide6.QtCore import Qt
from PySide6.QtGui import QGuiApplication

from size_grip import SizeGrip
from options_window import OptionsWindow
from about_window import AboutWindow
from data import Data


class Ruler(QWidget):
    def __init__(self):

        super(Ruler, self).__init__()

        self.old_position = None  # is used for dragging of the window
        self.setMinimumWidth(50)
        self.setMinimumHeight(50)

        # load the options
        self.data = Data()

        # main widget
        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.constructContextMenu)

        self.setWindowTitle('Screen Ruler')
        self.resize(self.data.get('ruler_width'),
                    self.data.get('ruler_height'))
        self.setMouseTracking(True)
        self.setAttribute(Qt.WA_TranslucentBackground, True)
        self.setGeometry(
            QStyle.alignedRect(Qt.LeftToRight, Qt.AlignCenter, self.size(),
                               QGuiApplication.primaryScreen().availableGeometry()))

        windowFlags = Qt.CustomizeWindowHint | Qt.FramelessWindowHint

        leftResize = SizeGrip(self, True)
        rightResize = SizeGrip(self, False)

        self.left_resize = leftResize
        self.right_resize = rightResize

        if self.data.get('always_above'):
            windowFlags = windowFlags | Qt.WindowStaysOnTopHint
        self.setWindowFlags(
            windowFlags)  # Turns off the default window title hints

        # initialize the secondary windows
        self.about_window = AboutWindow()
        self.options_window = OptionsWindow(self)

        if self.data.get('options_opened'):
            self.openOptions()

    def resizeEvent(self, event=None):
        size = self.size()
        width = size.width()
        height = size.height()
        leftResize = self.left_resize
        rightResize = self.right_resize
        length = leftResize.length

        if self.data.get('horizontal_orientation'):
            rightResize.move(width - length, 0)

            leftResize.resize(length, height)
            rightResize.resize(length, height)

            # vertical orientation
        else:
            rightResize.move(0, size.height() - rightResize.length)

            leftResize.resize(width, length)
            rightResize.resize(width, length)

    def paintEvent(self, event):

        paint = QPainter()
        paint.begin(self)
        paint.save()

        size = self.size()
        width = size.width()
        height = size.height()
        units = self.data.get('units')

        proportion = self.getProportion()

        horizontalOrientation = self.data.get('horizontal_orientation')

        if horizontalOrientation:

            rulerLength = width
            traceLengthLimit = height

        else:  # vertical orientation
            rulerLength = height
            traceLengthLimit = width

            paint.translate(width, 0)
            paint.rotate(90)

            # the length of the traces (lines)
        small = traceLengthLimit / 6
        medium = traceLengthLimit / 4
        large = traceLengthLimit / 3

        limit = rulerLength / proportion

        # draw less lines for centimeters
        if units == 'cm':
            step = 10

        else:
            step = 5

        # begin drawing
        fontSize = 10
        font = QFont('Serif', fontSize)
        fontMetrics = QFontMetrics(font)

        # draw background
        background = self.data.get('background_color')
        paint.fillRect(0, 0, rulerLength, traceLengthLimit, background)

        # draw the lines
        paint.setPen(self.data.get('lines_color'))
        paint.setFont(font)

        # the builtin range() doesn't support floats
        def float_range(current, end, step):
            while current < end:
                yield current
                current += step

            # we skip 0 and start in the first step, since there's no point in drawing the first line/text (it would appear cut off, since we're at the limit)

        for a in float_range(step, limit, step):

            position = a * proportion

            if (a % 100) == 0:
                lineLength = large

                if units == 'px':
                    text = '{}{}'.format(str(a), units)
                else:
                    text = '{}{}'.format(str(int(a / 100)), units)

                textWidth = fontMetrics.boundingRect(text).width()

                paint.drawText(position - textWidth / 2,
                               traceLengthLimit / 2 + fontSize / 2, text)

            elif (a % 50) == 0:
                lineLength = large

                # since 'cm' has a different step compared to the other units
                if units == 'cm':
                    lineLength = medium

            elif (a % 25) == 0:
                lineLength = medium

            else:
                lineLength = small

            paint.drawLine(position, 0, position, lineLength)
            paint.drawLine(position, traceLengthLimit, position,
                           traceLengthLimit - lineLength)

        # paint the division lines

        if self.data.get('division_lines'):
            paint.setPen(self.data.get('divisions_color'))
            halfPoint = rulerLength / 2
            quarterPoint = rulerLength / 4
            threeQuarterPoint = 3 / 4 * rulerLength

            paint.drawLine(quarterPoint, 0, quarterPoint, traceLengthLimit)
            paint.drawLine(halfPoint, 0, halfPoint, traceLengthLimit)
            paint.drawLine(threeQuarterPoint, 0,
                           threeQuarterPoint, traceLengthLimit)

        paint.restore()
        paint.end()

    def mousePressEvent(self, event):

        self.old_position = event.globalPos()

        button = event.button()

        if button == Qt.MiddleButton:
            self.rotate(QCursor.pos())

    def mouseMoveEvent(self, event, fromSizeGrip=False):

        buttons = event.buttons()

        # if we're on top of a SizeGrip, then clicking means we're resizing, and not moving the ruler
        if not fromSizeGrip and (buttons & Qt.LeftButton):

            position = event.globalPos()
            diff = position - self.old_position

            self.move(self.x() + diff.x(), self.y() + diff.y())

            self.old_position = position

        if self.options_window:
            pos = self.pos()

            if self.data.get('horizontal_orientation'):
                distance = event.globalX() - pos.x()

            else:
                distance = event.globalY() - pos.y()

            unit = self.data.get('units')

            if unit != 'px':
                distance /= 100
                value = distance / self.getProportion()
                string = '{:.2f} {}'

            else:
                value = distance  # its already in pixels
                string = '{} {}'

            self.options_window.setCurrentLength(string.format(value, unit))

    def keyPressEvent(self, event):

        key = event.key()
        modifiers = event.modifiers()

        if key == Qt.Key_O and modifiers == Qt.AltModifier:
            self.openOptions()

        elif key == Qt.Key_R and modifiers == Qt.AltModifier:
            self.rotate(QCursor.pos())

    def constructContextMenu(self, position):
        globalPosition = self.mapToGlobal(position)

        menu = QMenu()

        optionsEntry = QAction('Options', menu)
        optionsEntry.setData(self.openOptions)

        def rotate():
            mousePosition = QCursor.pos()

            self.rotate(mousePosition)

        rotateEntry = QAction('Rotate', menu)
        rotateEntry.setData(rotate)

        aboutEntry = QAction('About', menu)
        aboutEntry.setData(self.openAbout)

        quitEntry = QAction('Close', menu)
        quitEntry.setData(self.quit)

        menu.addAction(optionsEntry)
        menu.addAction(rotateEntry)
        menu.addAction(aboutEntry)
        menu.addAction(quitEntry)

        selectedItem = menu.exec_(globalPosition)
        """:type : QAction"""

        if selectedItem:
            selectedItem.data()()

    def openOptions(self):
        self.options_window.show()
        self.options_window.raise_()
        self.options_window.activateWindow()

    def rotate(self, mousePosition=None):

        self.data.update('horizontal_orientation', not self.data.get(
            'horizontal_orientation'))

        size = self.size()

        # position the ruler so that the resulting rotation is based on where the mouse is
        if mousePosition:
            rulerX = self.x()
            rulerY = self.y()
            mouseX = mousePosition.x()
            mouseY = mousePosition.y()

            self.move(mouseX - (mouseY - rulerY), mouseY - (mouseX - rulerX))

            # switch the width/height (to rotate 90 degrees)
        self.resize(size.height(), size.width())

        self.resizeEvent()

        # if the options window is opened, update it
        if self.options_window:
            self.options_window.updateOrientation(
                self.data.get('horizontal_orientation'))

    def openAbout(self):
        self.about_window.show()
        self.about_window.raise_()
        self.about_window.activateWindow()

    def getProportion(self):
        units = self.data.get('units')

        if units == 'cm':
            # 1 mm -> something pixel
            # width mm -> width pixel
            pxToMm = 1 * self.width() / self.widthMM()
            proportion = pxToMm / 10  # from mm to cm

        elif units == 'inch':
            # we'll calculate from mm to inches
            pxToMm = 1 * self.width() / self.widthMM()

            # 1 inch 2.54 cm
            proportion = 0.254 * pxToMm

        else:  # pixels
            proportion = 1

        return proportion

    def quit(self):
        self.close()

    def closeEvent(self, event):
        self.data.save({
            'width': self.width(),
            'height': self.height(),
            'optionsOpened': self.options_window.isVisible()
        })
        event.accept()
        QApplication.quit()


def run():
    app = QApplication(sys.argv)

    ruler = Ruler()
    ruler.show()

    app.exec_()


if __name__ == '__main__':
    run()
