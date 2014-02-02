# python 3.3

"""
    - when resizing, don't move the ruler when it has reached the minimum width/height
    - right sizegrip isn't position correctly
"""

import sys

from PySide.QtGui import QApplication, QWidget, QPainter, QGridLayout, QFont, QFontMetrics, QMenu, QAction, QLabel, QButtonGroup, QRadioButton, QCheckBox, QColor
from PySide.QtCore import Qt

import color_button, size_grip



class Ruler( QWidget ):
    def __init__( self, appObject ):

        super( Ruler, self ).__init__()

        self.app_obj = appObject
        self.about_window = None
        self.options_window = None
        self.old_position = None    # is used for dragging of the window
        self.options = {
            'units': 'px',
            'always_above': False,
            'horizontal_orientation': True,
            'background_color': QColor( 222, 212, 33, 127 ),
            'lines_color': QColor( 0, 0, 0, 255 ),

            'ruler_width': 500,
            'ruler_height': 50,
            'ruler_position_x': -1, #HERE maybe its better if not saved?...
            'ruler_position_y': -1, # -1 means to not force a specific position

            'options_opened': False,
            'options_position_x': -1,
            'options_position_y': -1        #HERE same
        }


            # main widget

        self.setContextMenuPolicy( Qt.CustomContextMenu )
        self.customContextMenuRequested.connect( self.constructContextMenu )

        self.setWindowTitle( 'Screen Ruler' )
        self.resize( 500, 50 )
        self.setWindowFlags( Qt.CustomizeWindowHint | Qt.FramelessWindowHint )   # Turns off the default window title hints


        leftResize = size_grip.SizeGrip( self, True )
        rightResize = size_grip.SizeGrip( self, False )

        layout = QGridLayout()
        layout.setContentsMargins( 0, 0, 0, 0 )
        layout.setSpacing( 0 )

        layout.addWidget( leftResize, 0, 0, Qt.AlignTop | Qt.AlignLeft )
        layout.addWidget( rightResize, 0, 1, Qt.AlignTop | Qt.AlignRight )

        self.setLayout( layout )





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

            # draw background
        background = self.options[ 'background_color' ]
        paint.fillRect( 0, 0, width, height, background )

            # draw the lines
        paint.setPen( self.options[ 'lines_color' ] )
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


    def constructContextMenu( self, position ):
        globalPosition = self.mapToGlobal( position )

        menu = QMenu()

        optionsEntry = QAction( 'Options', menu )
        optionsEntry.setData( self.openOptions )

        rotateEntry = QAction( 'Rotate', menu )
        rotateEntry.setData( self.rotate )

        aboutEntry = QAction( 'About', menu )
        aboutEntry.setData( self.openAbout )

        quitEntry = QAction( 'Close', menu )
        quitEntry.setData( self.quit )

        menu.addAction( optionsEntry )
        menu.addAction( rotateEntry )
        menu.addAction( aboutEntry )
        menu.addAction( quitEntry )

        selectedItem = menu.exec_( globalPosition )
        """:type : QAction"""

        if selectedItem:
            selectedItem.data()()

    def openOptions( self ):

        optionsWindow = QWidget()
        optionsWindow.setWindowTitle( 'Options' )

            # first column
        unitsGroup = QButtonGroup( optionsWindow )
        pixels = QRadioButton( 'Pixels' )
        centimeters = QRadioButton( 'Centimeters' )
        inches = QRadioButton( 'Inches' )

        pixels.setChecked( True )

        unitsGroup.addButton( pixels )
        unitsGroup.addButton( centimeters )
        unitsGroup.addButton( inches )

            # second column
        alwaysAbove = QCheckBox( 'Always Above', optionsWindow )
        alwaysAbove.setChecked( False )

        orientationGroup = QButtonGroup( optionsWindow )
        horizontal = QRadioButton( 'Horizontal' )
        vertical = QRadioButton( 'Vertical' )

        horizontal.setChecked( True )

        orientationGroup.addButton( horizontal )
        orientationGroup.addButton( vertical )

            # third column
        backgroundColor = self.options[ 'background_color' ]
        linesColor = self.options[ 'lines_color' ]

        def updateBackgroundColor( newColor ):
            self.options[ 'background_color' ] = newColor
            self.update()

        def updateLinesColor( newColor ):
            self.options[ 'lines_color' ] = newColor
            self.update()

        backgroundColorElement = color_button.ColorButton( optionsWindow, 'Background', backgroundColor, updateBackgroundColor )
        linesColorElement = color_button.ColorButton( optionsWindow, 'Lines', linesColor, updateLinesColor )
        currentLength = QLabel( '0px' )

        layout = QGridLayout()

            # first column
        layout.addWidget( pixels, 0, 0 )
        layout.addWidget( centimeters, 1, 0 )
        layout.addWidget( inches, 2, 0 )

            # second column
        layout.addWidget( alwaysAbove, 0, 1 )
        layout.addWidget( horizontal, 1, 1 )
        layout.addWidget( vertical, 2, 1 )

            # third column
        layout.addWidget( backgroundColorElement, 0, 2 )
        layout.addWidget( linesColorElement, 1, 2 )
        layout.addWidget( currentLength, 2, 2 )

        optionsWindow.setLayout( layout )
        optionsWindow.show()

        self.options_window = optionsWindow


    def rotate( self ):
        pass


    def openAbout( self ):

        aboutWindow = QWidget()
        aboutWindow.setWindowTitle( 'About' )

        textElement = QLabel(
            "For more information, visit: bitbucket.org/drk4/screen_ruler\n\n"
            "You can find there a wiki (with the documentation), and an issues tracker,\n"
            "where you can write suggestions or problems with the application.\n\n"
            "Thanks for using this program." )
        textElement.setTextInteractionFlags( Qt.TextSelectableByMouse | Qt.TextSelectableByKeyboard )

        layout = QGridLayout()

        layout.addWidget( textElement )

        aboutWindow.setLayout( layout )
        aboutWindow.show()

        self.about_window = aboutWindow



    def quit( self ):

        self.app_obj.quit()



if __name__ == '__main__':

    app = QApplication( sys.argv )

    ruler = Ruler( app )
    ruler.show()

    app.exec_()