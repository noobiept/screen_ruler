#include "options.h"

#include "screen_ruler.h"
#include "main.h"
#include "configurations.h"


extern ScreenRuler* SCREEN_RULER;


extern Configurations CONFIGURATIONS;


Options::Options()
{
    // :: Background color :: //


backgroundColorLabel_ui.set_label( "Background color" );

backgroundColor_ui.set_title( "Select a color" );
backgroundColor_ui.set_rgba( CONFIGURATIONS.backgroundColor );

backgroundColor_ui.set_use_alpha( true );       //HERE n fica transparente.. se calhar por causa da Gtk::Window ?..


    // :: Number and lines color :: //

numberLinesColorLabel_ui.set_label( "Number/lines color" );

numberLinesColor_ui.set_title( "Select a color" );
numberLinesColor_ui.set_rgba( CONFIGURATIONS.numberLinesColor );

numberLinesColor_ui.set_use_alpha( true );


    // :: Always above :: //

alwaysAbove_ui.set_label( "Always above" );
alwaysAbove_ui.set_active( CONFIGURATIONS.isAlwaysAbove );


    // :: pixel / centimeter / inch :: //

     pixels_ui.set_label( "Pixels" );
centimeters_ui.set_label( "Centimeters" );
     inches_ui.set_label( "Inches" );

Gtk::RadioButton::Group unitsGroup = pixels_ui.get_group();

centimeters_ui.set_group( unitsGroup );
     inches_ui.set_group( unitsGroup );



if (CONFIGURATIONS.units == "pixels")
    {
    pixels_ui.set_active( true );
    }

else if (CONFIGURATIONS.units == "inches")
    {
    inches_ui.set_active( true );
    }

else
    {
    centimeters_ui.set_active( true );
    }


    // :: Orientation :: //

orientation_ui.set_label( "Orientation" );  //HERE por tipo bold...

horizontal_ui.set_label( "Horizontal" );
vertical_ui.set_label( "Vertical" );


Gtk::RadioButton::Group orientationGroup = horizontal_ui.get_group();

vertical_ui.set_group( orientationGroup );


if ( CONFIGURATIONS.hasHorizontalOrientation == false )
    {
    vertical_ui.set_active( true );
    }

else
    {
    horizontal_ui.set_active( true );
    }



    // :: Color containers :: //

backgroundColorContainer_ui.set_spacing( 5 );
backgroundColorContainer_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );

backgroundColorContainer_ui.pack_start( backgroundColorLabel_ui );
backgroundColorContainer_ui.pack_start( backgroundColor_ui );


numberLinesColorContainer_ui.set_spacing( 5 );
numberLinesColorContainer_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );

numberLinesColorContainer_ui.pack_start( numberLinesColorLabel_ui );
numberLinesColorContainer_ui.pack_start( numberLinesColor_ui );


    // :: Grid - the main container :: //

container_ui.set_row_spacing( 5 );
container_ui.set_column_spacing( 30 );
container_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );
//container_ui.set_column_homogeneous( true );


container_ui.attach( pixels_ui, 0, 0, 1, 1 );
container_ui.attach( centimeters_ui, 0, 1, 1, 1 );
container_ui.attach( inches_ui, 0, 2, 1, 1 );

container_ui.attach( orientation_ui, 1, 0, 1, 1 );
container_ui.attach( horizontal_ui, 1, 1, 1, 1 );
container_ui.attach( vertical_ui, 1, 2, 1, 1 );

container_ui.attach( backgroundColorContainer_ui, 2, 0, 1, 1 );
container_ui.attach( numberLinesColorContainer_ui, 2, 1, 1, 1 );
container_ui.attach( alwaysAbove_ui, 2, 2, 1, 1 );



    // :: Window :: //

add( container_ui );

set_title( "Options" );

set_resizable( false );

set_border_width( 10 );

show_all_children();


    // :: Events :: //

alwaysAbove_ui.signal_toggled().connect( sigc::mem_fun( *this, &Options::alwaysAboveEvents ) );

     pixels_ui.signal_clicked().connect( sigc::bind< std::string, Gtk::RadioButton* >( sigc::mem_fun( *this, &Options::onUnitsChange ), "pixels"     , &pixels_ui      ) );
centimeters_ui.signal_clicked().connect( sigc::bind< std::string, Gtk::RadioButton* >( sigc::mem_fun( *this, &Options::onUnitsChange ), "centimeters", &centimeters_ui ) );
     inches_ui.signal_clicked().connect( sigc::bind< std::string, Gtk::RadioButton* >( sigc::mem_fun( *this, &Options::onUnitsChange ), "inches"     , &inches_ui      ) );

horizontal_ui.signal_clicked().connect( sigc::bind< bool, Gtk::RadioButton* >( sigc::mem_fun( *this, &Options::onOrientationChange ), true,  &horizontal_ui ) );
  vertical_ui.signal_clicked().connect( sigc::bind< bool, Gtk::RadioButton* >( sigc::mem_fun( *this, &Options::onOrientationChange ), false, &vertical_ui   ) );


 backgroundColor_ui.signal_color_set().connect( sigc::mem_fun(*this, &Options::backgroundColorEvents  ) );
numberLinesColor_ui.signal_color_set().connect( sigc::mem_fun(*this, &Options::numberLinesColorEvents ) );
}



void Options::onUnitsChange(std::string unit, Gtk::RadioButton* button)
{
    //when you click on a RadioButton, you can get two events, the other being from the previous active RadioButton
    //or just one, if this RadioButton was already active
    //we only care about the one that is being activated
if ( button->get_active() == false )
    {
    return;
    }

SCREEN_RULER->setUnits( unit );

SCREEN_RULER->draw.forceReDraw();
}



void Options::onOrientationChange( bool horizontalOrientation, Gtk::RadioButton* button )
{
    //for the same reason as in Options::onUnitsChange()
if ( button->get_active() == false )
    {
    return;
    }

setOrientation( horizontalOrientation, true );
}




void Options::alwaysAboveEvents()
{
setAlwaysAbove( alwaysAbove_ui.get_active() );
}



void Options::backgroundColorEvents()
{
Gdk::RGBA color = backgroundColor_ui.get_rgba();

CONFIGURATIONS.backgroundColor = color;

SCREEN_RULER->draw.forceReDraw();
}


void Options::numberLinesColorEvents()
{
Gdk::RGBA color = numberLinesColor_ui.get_rgba();

CONFIGURATIONS.numberLinesColor = color;

SCREEN_RULER->draw.forceReDraw();
}


/*
    Sets the ruler to be always above the rest of the windows or not
 */

void Options::setAlwaysAbove( bool yesNo )
{
SCREEN_RULER->set_keep_above( yesNo );

    //see if the CheckButton is in the right state
if ( alwaysAbove_ui.get_active() != yesNo )
    {
    alwaysAbove_ui.set_active( yesNo );
    }

    //update the configurations
CONFIGURATIONS.isAlwaysAbove = yesNo;
}




void Options::setOrientation( bool horizontalOrientation, bool toMiddleOfScreen )
{
    //change the orientation
SCREEN_RULER->setHorizontalOrientation( horizontalOrientation, toMiddleOfScreen );

    //and update the ui
if (horizontalOrientation == true)
    {
    horizontal_ui.set_active( true );
    }

else
    {
    vertical_ui.set_active( true );
    }
}




void Options::setBackgroundColor( Gdk::RGBA color )
{

}



void Options::setNumberLinesColor( Gdk::RGBA color )
{

}


