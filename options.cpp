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


    // :: Number and lines color :: //

numberLinesColorLabel_ui.set_label( "Number and lines color" );

numberLinesColor_ui.set_title( "Select a color" );
numberLinesColor_ui.set_rgba( CONFIGURATIONS.numberLinesColor );



    // :: Always above :: //


alwaysAbove_ui.set_label( "Always above" );
alwaysAbove_ui.set_active( CONFIGURATIONS.isAlwaysAbove );


    // :: pixel / centimeter / inch :: //

     pixels_ui.set_label( "Pixels" );
centimeters_ui.set_label( "Centimeters" );
     inches_ui.set_label( "Inches" );

Gtk::RadioButton::Group group = pixels_ui.get_group();

centimeters_ui.set_group( group );
     inches_ui.set_group( group );



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


    // :: First line - container :: //


firstLine_ui.set_spacing( 10 );

firstLine_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );

firstLine_ui.add( backgroundColorLabel_ui  );
firstLine_ui.add( backgroundColor_ui       );
firstLine_ui.add( numberLinesColorLabel_ui );
firstLine_ui.add( numberLinesColor_ui      );

    // :: Second line - container :: //

//secondLine_ui.set_homogeneous( true );

secondLine_ui.set_spacing( 10 );
secondLine_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );



secondLine_ui.add( pixels_ui      );
secondLine_ui.add( centimeters_ui );
secondLine_ui.add( inches_ui      );
secondLine_ui.add( alwaysAbove_ui );


    // :: Grid - the main container :: //

container_ui.set_row_spacing( 20 );
container_ui.set_orientation( Gtk::ORIENTATION_VERTICAL );

container_ui.add( firstLine_ui  );
container_ui.add( secondLine_ui );



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
}







void Options::alwaysAboveEvents()
{
SCREEN_RULER->set_keep_above( alwaysAbove_ui.get_active() );
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

