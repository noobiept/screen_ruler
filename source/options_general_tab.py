from PySide2.QtWidgets import QWidget, QButtonGroup, QRadioButton, QCheckBox, QLabel, QGridLayout, QLayout, QDialog, QTabWidget, QVBoxLayout
from PySide2.QtCore import Qt


class OptionsGeneralTab(QWidget):
    def __init__(self, rulerObject, parent=None):
        QWidget.__init__(self, parent)

        # units (cm/px/inch)
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

        # orientation (horizontal/vertical)
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

        # length settings (always above + division lines + current length)
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

        divisionLines = QCheckBox('1/2 1/4 3/4', self)
        divisionLines.setChecked(rulerObject.options['division_lines'])

        def divisionLinesSetting():
            rulerObject.options['division_lines'] = divisionLines.isChecked(
            )
            rulerObject.update()

        divisionLines.clicked.connect(divisionLinesSetting)

        currentLength = QLabel('0px')
        currentLength.setAlignment(Qt.AlignCenter)

        # setup the layout
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
        layout.addWidget(horizontal, 0, 1)
        layout.addWidget(vertical, 1, 1)

        # third column
        layout.addWidget(alwaysAbove, 0, 2)
        layout.addWidget(divisionLines, 1, 2)
        layout.addWidget(currentLength, 2, 2)

        layout.setSizeConstraint(QLayout.SetFixedSize)

        self.current_length = currentLength
        self.vertical = vertical
        self.horizontal = horizontal
        self.setLayout(layout)

    def setCurrentLength(self, length):
        self.current_length.setText(length)

    def updateOrientation(self, isHorizontal):

        if isHorizontal:
            self.horizontal.setChecked(True)
            self.selected_orientation = self.horizontal_string

        else:
            self.vertical.setChecked(True)
            self.selected_orientation = self.vertical_string
