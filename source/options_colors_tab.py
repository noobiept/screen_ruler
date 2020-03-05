from PySide2.QtWidgets import QWidget, QGridLayout, QLayout

from color_button import ColorButton


class OptionsColorsTab(QWidget):
    def __init__(self, ruler, parent=None):
        QWidget.__init__(self, parent)

        # colors settings
        backgroundColor = ruler.data.get('background_color')
        linesColor = ruler.data.get('lines_color')
        divisionsColor = ruler.data.get('divisions_color')

        def updateBackgroundColor(newColor):
            ruler.data.update('background_color', newColor)
            ruler.update()

        def updateLinesColor(newColor):
            ruler.data.update('lines_color', newColor)
            ruler.update()

        def updateDivisionsColor(newColor):
            ruler.data.update('divisions_color', newColor)
            ruler.update()

        backgroundColorElement = ColorButton(
            self, 'Background', backgroundColor, updateBackgroundColor)
        linesColorElement = ColorButton(
            self, 'Lines', linesColor, updateLinesColor)
        divisionsColorElement = ColorButton(
            self, 'Divisions', divisionsColor, updateDivisionsColor)

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
        layout.addWidget(backgroundColorElement, 1, 0)

        # second column
        layout.addWidget(linesColorElement, 1, 1)

        # third column
        layout.addWidget(divisionsColorElement, 1, 2)

        layout.setSizeConstraint(QLayout.SetFixedSize)
        self.setLayout(layout)
