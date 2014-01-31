# python 3.3

import sys

from PySide.QtGui import QApplication, QWidget, QPainter, QGridLayout, QSizeGrip, QFont, QFontMetrics
from PySide.QtCore import Qt


class Ruler( QWidget ):
    def __init__( self, parent= None ):

        super( Ruler, self ).__init__( parent )

        self.old_position = None    # is used for dragging of the window

        resize = QSizeGrip( self )

        layout = QGridLayout()

        layout.addWidget( resize, 0, 0, Qt.AlignBottom | Qt.AlignLeft )

        self.setLayout( layout )

            # main widget
        self.setWindowTitle( 'Screen Ruler' )
        self.resize( 500, 50 )
        self.setWindowFlags( Qt.CustomizeWindowHint | Qt.FramelessWindowHint )   # Turns off the default window title hints

            # set events


    def paintEvent(self, event):

        size = self.size()
        width = size.width()
        height = size.height()
        units = 'px'    # pixels

            # depends on the unit
        proportion = 1  # 1 --> pixel
        horizontalOrientation = True

        if horizontalOrientation:

            rulerLength = width
            traceLengthLimit = height

        else:   # vertical orientation
            rulerLength = height
            traceLengthLimit = width


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
        paint = QPainter()
        fontSize = 10
        font = QFont( 'Serif', fontSize )
        fontMetrics = QFontMetrics( font )

        paint.begin( self )
        paint.setFont( font )

            # the builtin range() doesn't support floats
        def float_range(current, end, step):
            while current < end:
                yield current
                current += step

        for a in float_range( 0, limit, step ):

            if (a % 100) == 0:
                lineLength = large

                    # don't draw the text for the first one (since its going to be cut anyway)
                if a != 0:
                    text = '{}{}'.format( str( a ), units )
                    textWidth = fontMetrics.width( text )

                    paint.drawText( a - textWidth / 2, traceLengthLimit / 2 + fontSize / 2, text )

            elif (a % 50) == 0:
                lineLength = large

                    # since has different step?.. #HERE
                if units == 'cm':
                    lineLength = medium

            elif (a % 25) == 0:
                lineLength = medium

            else:
                lineLength = small


            paint.drawLine( a, 0, a, lineLength )
            paint.drawLine( a, traceLengthLimit, a,  traceLengthLimit - lineLength )
        paint.end()


    def mousePressEvent(self, event):

        self.old_position = event.globalPos()

        button = event.button()

        if button == Qt.RightButton:
            print('right button')

        elif button == Qt.LeftButton:
            print('left button')

        print( 'clicked' )

    def mouseMoveEvent( self, event ):

        position = event.globalPos()
        diff = position - self.old_position

        self.move( self.x() + diff.x(), self.y() + diff.y() )

        self.old_position = position

    def keyPressEvent(self, *args, **kwargs):
        print('key pressed')

    def keyReleaseEvent(self, *args, **kwargs):
        print('key release')



if __name__ == '__main__':

    app = QApplication( sys.argv )

    ruler = Ruler()
    ruler.show()

    app.exec_()