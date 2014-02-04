# python 3.3

"""
    - when resizing, don't move the ruler when it has reached the minimum width/height

    - the proportion is not well calculated for centimeters in windows (you get wrong values for .widthMM() call)
        - works fine in linux though

    - save/load the configurations to an external file
    - transparency of the background (need to apply to the main window, not just in rgba())
    - when rotating, move the ruler so that it appears on top of the mouse (or if pressed from the menu appear on the center of screen?...)
"""

import sys

from PySide.QtGui import QApplication, QWidget, QPainter, QGridLayout, QFont, QFontMetrics, QMenu, QAction, QLabel, QColor, QLayout
from PySide.QtCore import Qt

import size_grip, options_window



class Ruler( QWidget ):
    def __init__( self ):

        super( Ruler, self ).__init__()

        self.about_window = None
        self.options_window = None
        self.old_position = None    # is used for dragging of the window
        self.setMinimumWidth( 50 )
        self.setMinimumHeight( 50 )
        self.options = {
            'units': 'px',      # px / cm / inch
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
        self.setMouseTracking( True )

        windowFlags = Qt.CustomizeWindowHint | Qt.FramelessWindowHint

        if self.options[ 'always_above' ]:
            windowFlags = windowFlags | Qt.WindowStaysOnTopHint
        self.setWindowFlags( windowFlags )   # Turns off the default window title hints

        leftResize = size_grip.SizeGrip( self, True )
        rightResize = size_grip.SizeGrip( self, False )

        self.left_resize = leftResize
        self.right_resize = rightResize


    def resizeEvent(self, event= None):
        size = self.size()
        width = size.width()
        height = size.height()
        leftResize = self.left_resize
        rightResize = self.right_resize
        length = leftResize.length

        if self.options[ 'horizontal_orientation' ]:
            rightResize.move( width - length, 0 )

            leftResize.resize( length, height )
            rightResize.resize( length, height )

            # vertical orientation
        else:
            rightResize.move( 0, size.height() - rightResize.length )

            leftResize.resize( width, length )
            rightResize.resize( width, length )



    def paintEvent(self, event):

        paint = QPainter()
        paint.begin( self )
        paint.save()

        size = self.size()
        width = size.width()
        height = size.height()
        units = self.options[ 'units' ]

        proportion = self.getProportion()

        horizontalOrientation = self.options[ 'horizontal_orientation' ]

        if horizontalOrientation:

            rulerLength = width
            traceLengthLimit = height

        else:   # vertical orientation
            rulerLength = height
            traceLengthLimit = width

            paint.translate( width, 0 )
            paint.rotate( 90 )


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
        font = QFont( 'Serif', fontSize )
        fontMetrics = QFontMetrics( font )


            # draw background
        background = self.options[ 'background_color' ]
        paint.fillRect( 0, 0, rulerLength, traceLengthLimit, background )

            # draw the lines
        paint.setPen( self.options[ 'lines_color' ] )
        paint.setFont( font )

            # the builtin range() doesn't support floats
        def float_range(current, end, step):
            while current < end:
                yield current
                current += step

            # we skip 0 and start in the first step, since there's no point in drawing the first line/text (it would appear cut off, since we're at the limit)
        for a in float_range( step, limit, step ):

            position = a * proportion

            if (a % 100) == 0:
                lineLength = large

                if units == 'px':
                    text = '{}{}'.format( str( a ), units )
                else:
                    text = '{}{}'.format( str( int(a / 100) ), units )

                textWidth = fontMetrics.width( text )

                paint.drawText( position - textWidth / 2, traceLengthLimit / 2 + fontSize / 2, text )

            elif (a % 50) == 0:
                lineLength = large

                    # since has different step?.. #HERE
                if units == 'cm':
                    lineLength = medium

            elif (a % 25) == 0:
                lineLength = medium

            else:
                lineLength = small


            paint.drawLine( position, 0, position, lineLength )
            paint.drawLine( position, traceLengthLimit, position, traceLengthLimit - lineLength )

        paint.restore()
        paint.end()


    def mousePressEvent(self, event):

        self.old_position = event.globalPos()

        button = event.button()

        if button == Qt.MidButton:
            self.rotate()


    def mouseMoveEvent( self, event, fromSizeGrip= False ):

        buttons = event.buttons()

            # if we're on top of a SizeGrip, then clicking means we're resizing, and not moving the ruler
        if not fromSizeGrip and (buttons & Qt.LeftButton):

            position = event.globalPos()
            diff = position - self.old_position

            self.move( self.x() + diff.x(), self.y() + diff.y() )

            self.old_position = position


        if self.options_window:
            pos = self.pos()

            if self.options[ 'horizontal_orientation' ]:
                distance = event.globalX() - pos.x()


            else:
                distance = event.globalY() - pos.y()


            unit = self.options[ 'units' ]

            if unit != 'px':
                distance /= 100
                value = distance / self.getProportion()
                string = '{:.2f} {}'

            else:
                value = distance    # its already in pixels
                string = '{} {}'

            self.options_window.setCurrentLength( string.format( value, unit ) )


    def keyPressEvent( self, event ):

        key = event.key()
        modifiers = event.modifiers()

        if key == Qt.Key_O and modifiers == Qt.AltModifier:
            self.openOptions()

        elif key == Qt.Key_R and modifiers == Qt.AltModifier:
            self.rotate()


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

            # already opened
        if self.options_window:
            self.options_window.raise_()
            self.options_window.activateWindow()
            return

        optionsWindow = options_window.OptionsWindow( self )

            # reset the self.options_window variable, to tell when the options window is opened or not
        def closedOptionsWindow( event ):
            self.options_window = None

            event.accept()


        optionsWindow.closeEvent = closedOptionsWindow

        self.options_window = optionsWindow


    def rotate( self ):

        self.options[ 'horizontal_orientation' ] = not self.options[ 'horizontal_orientation' ]

        size = self.size()

            # switch the width/height (to rotate 90 degrees)
        self.resize( size.height(), size.width() )
        self.resizeEvent()

            # if the options window is opened, update it
        if self.options_window:
            self.options_window.updateOrientation( self.options[ 'horizontal_orientation' ] )



    def openAbout( self ):

            # already opened
        if self.about_window:
            self.about_window.raise_()
            self.about_window.activateWindow()
            return

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
        layout.setSizeConstraint( QLayout.SetFixedSize )

        aboutWindow.setLayout( layout )

        def keyPress( event ):

            if event.key() == Qt.Key_Escape:
                aboutWindow.close()

        aboutWindow.keyPressEvent = keyPress
        aboutWindow.show()

            # reset the self.about_window variable, to tell when the about window is opened or not
        def closedAboutWindow( event ):
            self.about_window = None

            event.accept()

        aboutWindow.closeEvent = closedAboutWindow

        self.about_window = aboutWindow


    def getProportion( self ):
        units = self.options[ 'units' ]

        if units == 'cm':
                # 1 mm -> something pixel
                # width mm -> width pixel
            pxToMm = 1 * self.width() / self.widthMM()
            proportion = pxToMm / 10    # from mm to cm

        elif units == 'inch':
                # we'll calculate from mm to inches
            pxToMm = 1 * self.width() / self.widthMM()

                # 1 inch 2.54 cm
            proportion = 0.254 * pxToMm

        else:   # pixels
            proportion = 1

        return proportion



    def quit( self ):

        QApplication.quit()



if __name__ == '__main__':

    app = QApplication( sys.argv )

    ruler = Ruler()
    ruler.show()

    app.exec_()