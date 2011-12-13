#include "options.h"

Options::Options()
{
    // :: Angle :: //

angleLabel_ui.set_label( "Angle" );

Glib::RefPtr<Gtk::Adjustment> angleAdjustment (

          Gtk::Adjustment::create(0,
                                  0,
                                  359, 1, 5, 0)

                                              );

angle_ui.set_adjustment( angleAdjustment );

angle_ui.set_numeric( true );
angle_ui.set_digits( 3 );


    // :: Always above :: //


alwaysAbove_ui.set_label( "Always above" );
alwaysAbove_ui.set_active( false );


    // :: pixel or centimeter :: //

pixels_ui.set_label( "Pixels" );
centimeters_ui.set_label( "Centimeters" );

Gtk::RadioButton::Group group = pixels_ui.get_group();

centimeters_ui.set_group( group );

pixels_ui.set_active( true );



    // :: First line - container :: //


firstLine_ui.set_spacing( 10 );

firstLine_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );

firstLine_ui.add( angleLabel_ui );
firstLine_ui.add( angle_ui );
firstLine_ui.add( alwaysAbove_ui );

    // :: Second line - container :: //

secondLine_ui.set_homogeneous( true );

secondLine_ui.set_spacing( 10 );
secondLine_ui.set_orientation( Gtk::ORIENTATION_HORIZONTAL );



secondLine_ui.add( pixels_ui );
secondLine_ui.add( centimeters_ui );


    // :: Grid - the main container :: //

container_ui.set_row_spacing( 20 );
container_ui.set_orientation( Gtk::ORIENTATION_VERTICAL );

container_ui.add( firstLine_ui );
container_ui.add( secondLine_ui );



    // :: Window :: //

add( container_ui );

set_title( "Options" );

set_resizable( false );

set_border_width( 10 );

show_all_children();


    // :: Events :: //

alwaysAbove_ui.signal_toggled().connect( sigc::mem_fun( *this, &Options::alwaysAboveEvents ) );
}







void Options::open()
{
    //shows the window if its closed, or brings it to the front if its already opened
present();
}




sigc::signal<void, int> Options::signal_onAlwaysAboveChange()
{
return the_signal_onAlwaysAboveChange;
}






void Options::alwaysAboveEvents()
{
the_signal_onAlwaysAboveChange.emit ( alwaysAbove_ui.get_active() );
}


