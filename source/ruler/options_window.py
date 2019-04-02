from PySide2.QtWidgets import QWidget, QButtonGroup, QRadioButton, QCheckBox, QLabel, QGridLayout, QLayout
from PySide2.QtCore import Qt

from . import color_button


class OptionsWindow(QWidget):
    def __init__(self, rulerObject):
        super(OptionsWindow, self).__init__(None)

        self.setWindowTitle('Options')

        # first column (units)
        unitsGroup = QButtonGroup(self)
        pixels = QRadioButton('Pixels')
        pixels.unit = 'px'
        centimeters = QRadioButton('Centimeters')
        centimeters.unit = 'cm'
        inches = QRadioButton('Inches')
        inches.unit = 'inch'

        selectedUnit = rulerObject.options['units']

        if selectedUnit == 'cm':
            centimeters.setChecked(True)

        elif selectedUnit == 'inch':
            inches.setChecked(True)

        else:
            pixels.setChecked(True)

        unitsGroup.addButton(pixels)
        unitsGroup.addButton(centimeters)
        unitsGroup.addButton(inches)

        def changeUnits(radioButton):
            nonlocal selectedUnit
            newUnit = radioButton.unit

            if newUnit != selectedUnit:
                rulerObject.options['units'] = newUnit
                selectedUnit = newUnit
                rulerObject.update()

        unitsGroup.buttonClicked.connect(changeUnits)

        # second column (always above + orientation)
        alwaysAbove = QCheckBox('Always Above', self)
        alwaysAbove.setChecked(rulerObject.options['always_above'])

        def alwaysAboveSetting():

            if alwaysAbove.isChecked():

                rulerObject.options['always_above'] = True
                rulerObject.setWindowFlags(rulerObject.windowFlags()
                                           | Qt.WindowStaysOnTopHint)

            else:
                rulerObject.options['always_above'] = False
                rulerObject.setWindowFlags(
                    rulerObject.windowFlags() & ~Qt.WindowStaysOnTopHint)

            rulerObject.show()

        alwaysAbove.clicked.connect(alwaysAboveSetting)
        horizontalOrientation = rulerObject.options['horizontal_orientation']

        orientationGroup = QButtonGroup(self)
        self.horizontal_string = 'Horizontal'
        self.vertical_string = 'Vertical'
        horizontal = QRadioButton(self.horizontal_string)
        vertical = QRadioButton(self.vertical_string)

        if horizontalOrientation:
            horizontal.setChecked(True)
            self.selected_orientation = self.horizontal_string

        else:
            vertical.setChecked(True)
            self.selected_orientation = self.vertical_string

        def changeOrientation(radioButton):
            text = radioButton.text()

            if self.selected_orientation != text:

                self.selected_orientation = text
                rulerObject.rotate()

        orientationGroup.addButton(horizontal)
        orientationGroup.addButton(vertical)
        orientationGroup.buttonClicked.connect(changeOrientation)

        # third column (color + current length)
        backgroundColor = rulerObject.options['background_color']
        linesColor = rulerObject.options['lines_color']

        def updateBackgroundColor(newColor):
            rulerObject.options['background_color'] = newColor
            rulerObject.update()

        def updateLinesColor(newColor):
            rulerObject.options['lines_color'] = newColor
            rulerObject.update()

        backgroundColorElement = color_button.ColorButton(
            self, 'Background', backgroundColor, updateBackgroundColor)
        linesColorElement = color_button.ColorButton(self, 'Lines', linesColor,
                                                     updateLinesColor)
        currentLength = QLabel('0px')
        currentLength.setAlignment(Qt.AlignCenter)

        layout = QGridLayout()
        layout.setSpacing(10)

        # set the same width/height for all the columns/rows
        columnWidth = 120
        rowHeight = 25
        layout.setRowMinimumHeight(0, rowHeight)
        layout.setRowMinimumHeight(1, rowHeight)
        layout.setRowMinimumHeight(2, rowHeight)
        layout.setColumnMinimumWidth(0, columnWidth)
        layout.setColumnMinimumWidth(1, columnWidth)
        layout.setColumnMinimumWidth(2, columnWidth)

        # first column
        layout.addWidget(pixels, 0, 0)
        layout.addWidget(centimeters, 1, 0)
        layout.addWidget(inches, 2, 0)

        # second column
        layout.addWidget(alwaysAbove, 0, 1)
        layout.addWidget(horizontal, 1, 1)
        layout.addWidget(vertical, 2, 1)

        # third column
        layout.addWidget(backgroundColorElement, 0, 2)
        layout.addWidget(linesColorElement, 1, 2)
        layout.addWidget(currentLength, 2, 2)

        layout.setSizeConstraint(QLayout.SetFixedSize)
        self.setLayout(layout)

        self.show()

        self.current_length = currentLength
        self.vertical = vertical
        self.horizontal = horizontal

    def setCurrentLength(self, length):

        self.current_length.setText(length)

    def updateOrientation(self, isHorizontal):

        if isHorizontal:
            self.horizontal.setChecked(True)
            self.selected_orientation = self.horizontal_string

        else:
            self.vertical.setChecked(True)
            self.selected_orientation = self.vertical_string

    def keyPressEvent(self, event):

        if event.key() == Qt.Key_Escape:
            self.close()
