#!/usr/bin/env python3
"""
    - when resizing, don't move the ruler when it has reached the minimum width/height
        - happens on the left SizeGrip, since we're moving it when resizing
        - difficult to know when .resize() isn't resizing

    - the proportion is not well calculated for centimeters/inches in windows (you get wrong values for .widthMM() call)
        - works fine in linux though
"""

import sys
import json

from PySide2.QtWidgets import QApplication, QWidget, QGridLayout, QMenu, QAction, QLabel, QLayout, QStyle
from PySide2.QtGui import QPainter, QFont, QFontMetrics, QColor, QCursor
from PySide2.QtCore import Qt

from . import size_grip
from . import options_window


class Ruler(QWidget):
    def __init__(self):

        super(Ruler, self).__init__()

        self.about_window = None
        self.options_window = None
        self.old_position = None  # is used for dragging of the window
        self.setMinimumWidth(50)
        self.setMinimumHeight(50)
        self.options = {
            'units': 'px',  # px / cm / inch
            'always_above': False,
            'horizontal_orientation': True,
            'background_color': QColor(222, 212, 33, 210),
            'lines_color': QColor(0, 0, 0, 255),
            'ruler_width': 500,
            'ruler_height': 50,
            'options_opened': False
        }

        # load the options
        self.load()

        # main widget

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.constructContextMenu)

        self.setWindowTitle('Screen Ruler')
        self.resize(self.options['ruler_width'], self.options['ruler_height'])
        self.setMouseTracking(True)
        self.setAttribute(Qt.WA_TranslucentBackground, True)
        self.setGeometry(
            QStyle.alignedRect(Qt.LeftToRight, Qt.AlignCenter, self.size(),
                               QApplication.desktop().availableGeometry()))

        windowFlags = Qt.CustomizeWindowHint | Qt.FramelessWindowHint

        leftResize = size_grip.SizeGrip(self, True)
        rightResize = size_grip.SizeGrip(self, False)

        self.left_resize = leftResize
        self.right_resize = rightResize

        if self.options['always_above']:
            windowFlags = windowFlags | Qt.WindowStaysOnTopHint
        self.setWindowFlags(
            windowFlags)  # Turns off the default window title hints

        if self.options['options_opened']:
            self.openOptions()

    def resizeEvent(self, event=None):
        size = self.size()
        width = size.width()
        height = size.height()
        leftResize = self.left_resize
        rightResize = self.right_resize
        length = leftResize.length

        if self.options['horizontal_orientation']:
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
        units = self.options['units']

        proportion = self.getProportion()

        horizontalOrientation = self.options['horizontal_orientation']

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
        background = self.options['background_color']
        paint.fillRect(0, 0, rulerLength, traceLengthLimit, background)

        # draw the lines
        paint.setPen(self.options['lines_color'])
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

                textWidth = fontMetrics.width(text)

                paint.drawText(position - textWidth / 2,
                               traceLengthLimit / 2 + fontSize / 2, text)

            elif (a % 50) == 0:
                lineLength = large

                # since has different step?.. #HERE
                if units == 'cm':
                    lineLength = medium

            elif (a % 25) == 0:
                lineLength = medium

            else:
                lineLength = small

            paint.drawLine(position, 0, position, lineLength)
            paint.drawLine(position, traceLengthLimit, position,
                           traceLengthLimit - lineLength)

        paint.restore()
        paint.end()

    def mousePressEvent(self, event):

        self.old_position = event.globalPos()

        button = event.button()

        if button == Qt.MidButton:
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

            if self.options['horizontal_orientation']:
                distance = event.globalX() - pos.x()

            else:
                distance = event.globalY() - pos.y()

            unit = self.options['units']

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

        # already opened
        if self.options_window:
            self.options_window.raise_()
            self.options_window.activateWindow()
            return

        optionsWindow = options_window.OptionsWindow(self)

        # reset the self.options_window variable, to tell when the options window is opened or not
        def closedOptionsWindow(event):
            self.options_window = None

            event.accept()

        optionsWindow.closeEvent = closedOptionsWindow

        self.options_window = optionsWindow

    def rotate(self, mousePosition=None):

        self.options['horizontal_orientation'] = not self.options[
            'horizontal_orientation']

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
                self.options['horizontal_orientation'])

    def openAbout(self):

        # already opened
        if self.about_window:
            self.about_window.raise_()
            self.about_window.activateWindow()
            return

        aboutWindow = QWidget()
        aboutWindow.setWindowTitle('About')

        textElement = QLabel(
            "For more information, visit: bitbucket.org/drk4/screen_ruler\n\n"
            "You can find there a readme (with the documentation), the source code, and an issues tracker,\n"
            "where you can write suggestions or problems with the application.\n\n"
            "Thanks for using this program.")
        textElement.setTextInteractionFlags(Qt.TextSelectableByMouse
                                            | Qt.TextSelectableByKeyboard)

        layout = QGridLayout()
        layout.addWidget(textElement)
        layout.setSizeConstraint(QLayout.SetFixedSize)

        aboutWindow.setLayout(layout)

        def keyPress(event):

            if event.key() == Qt.Key_Escape:
                aboutWindow.close()

        aboutWindow.keyPressEvent = keyPress
        aboutWindow.show()

        # reset the self.about_window variable, to tell when the about window is opened or not
        def closedAboutWindow(event):
            self.about_window = None

            event.accept()

        aboutWindow.closeEvent = closedAboutWindow

        self.about_window = aboutWindow

    def getProportion(self):
        units = self.options['units']

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

    def save(self):
        # need to get the individual rgba() from the QColor so that we can serialize it into json
        background = self.options['background_color']
        lines = self.options['lines_color']

        self.options['background_color'] = {
            'red': background.red(),
            'green': background.green(),
            'blue': background.blue(),
            'alpha': background.alpha()
        }

        self.options['lines_color'] = {
            'red': lines.red(),
            'green': lines.green(),
            'blue': lines.blue(),
            'alpha': lines.alpha()
        }

        self.options['ruler_width'] = self.width()
        self.options['ruler_height'] = self.height()

        options = json.dumps(self.options)

        with open('config.txt', 'w', encoding='utf-8') as f:
            f.write(options)

    def load(self):

        try:
            with open('config.txt', 'r', encoding='utf-8') as f:
                try:
                    options = json.load(f)

                except ValueError:
                    return

                for key, value in options.items():
                    self.options[key] = value

                    # deal with the colors (init. the QColor() from)
                backgroundColor = self.options['background_color']
                linesColor = self.options['lines_color']

                if isinstance(backgroundColor, dict):
                    self.options['background_color'] = QColor(
                        backgroundColor['red'], backgroundColor['green'],
                        backgroundColor['blue'], backgroundColor['alpha'])

                if isinstance(linesColor, dict):
                    self.options['lines_color'] = QColor(
                        linesColor['red'], linesColor['green'],
                        linesColor['blue'], linesColor['alpha'])
        except FileNotFoundError:
            return

    def quit(self):

        self.close()

    def closeEvent(self, event):

        if self.options_window:
            self.options['options_opened'] = True
            self.options_window.close()
        else:
            self.options['options_opened'] = False

        if self.about_window:
            self.about_window.close()

        self.save()

        event.accept()


def run():
    app = QApplication(sys.argv)

    ruler = Ruler()
    ruler.show()

    app.exec_()
