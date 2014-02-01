# python 3.3

from PySide.QtGui import QPushButton, QWidget, QLabel, QGridLayout, QColorDialog, QColor

class ColorButton( QWidget ):

    def __init__( self, parent, text: str, color: QColor, onChange ):
        """
            onChange( QColor ) : called when a color is chosen. Receives the new color as argument
        """

        super( ColorButton, self ).__init__( parent )

            # reference to the color dialog that is opened
        self.color_dialog = None

            # elements
        button = QPushButton()
        label = QLabel( text )

        layout = QGridLayout()

        layout.addWidget( label, 0, 0 )
        layout.addWidget( button, 0, 1 )

        self.setLayout( layout )
        self.show()

        button.clicked.connect( self.onClick )

        self.button = button
        self.color = color
        self.on_change = onChange

        self.styleButton()


    def styleButton( self ):

        color = self.color

        self.button.setStyleSheet( 'background-color: rgba({},{},{},{});'.format( color.red(), color.green(), color.blue(), color.alpha() ) )


    def onClick( self ):

        colorDialog = QColorDialog()
        colorDialog.setOption( QColorDialog.ShowAlphaChannel )

        colorDialog.show()

        def colorSelected( color ):
            self.color = color
            self.styleButton()

            if self.on_change:
                self.on_change( self.color )

        colorDialog.currentColorChanged.connect( colorSelected )

        self.color_dialog = colorDialog