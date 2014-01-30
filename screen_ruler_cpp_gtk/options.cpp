/*

	Copyright - 2011 - Pedro Ferreira

	This file is part of Screen_ruler.

    Screen_ruler is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Screen_ruler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Screen_ruler.  If not, see <http://www.gnu.org/licenses/>.

 */



#include "options.h"

#include "screen_ruler.h"
#include "main.h"
#include "configurations.h"


extern ScreenRuler* SCREEN_RULER;


extern Configurations CONFIGURATIONS;


Options::Options()
{
    // :: Background color :: //

backgroundColorLabel_ui.set_label( "Background" );

backgroundColor_ui.set_title( "Select a color" );
backgroundColor_ui.set_rgba( CONFIGURATIONS.backgroundColor );

backgroundColor_ui.set_use_alpha( true );       //HERE n fica transparente.. se calhar por causa da Gtk::Window ?..


    // :: Number and lines color :: //

numberLinesColorLabel_ui.set_label( "Number/lines" );

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


    // :: Mouse Length  :: //


mouseLength_ui.set_label( "0px" );


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


container_ui.attach( pixels_ui, 0, 0, 1, 1 );
container_ui.attach( centimeters_ui, 0, 1, 1, 1 );
container_ui.attach( inches_ui, 0, 2, 1, 1 );

container_ui.attach( alwaysAbove_ui, 1, 0, 1, 1 );
container_ui.attach( horizontal_ui, 1, 1, 1, 1 );
container_ui.attach( vertical_ui, 1, 2, 1, 1 );

container_ui.attach( backgroundColorContainer_ui, 2, 0, 1, 1 );
container_ui.attach( numberLinesColorContainer_ui, 2, 1, 1, 1 );
container_ui.attach( mouseLength_ui, 2, 2, 1, 1);


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
CONFIGURATIONS.backgroundColor = color;

backgroundColor_ui.set_rgba( color );
}



void Options::setNumberLinesColor( Gdk::RGBA color )
{
CONFIGURATIONS.numberLinesColor = color;

numberLinesColor_ui.set_rgba( color );
}



/*
    Shows number of pixels (or whatever unit is set) from the origin to where the mouse is

    Only works when the mouse is over the ruler
 */

void Options::setMouseLength( int pixelLength )
{
std::string unit = SCREEN_RULER->getShortUnits();

double proportion = SCREEN_RULER->draw.getProportion();


double measure = pixelLength / proportion;

if ( unit != "px" )
    {
    measure /= 100;
    }

std::stringstream stream;

stream << measure << unit;


mouseLength_ui.set_label( stream.str() );
}
