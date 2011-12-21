#include "about.h"



About::About()
{


text_ui.set_label( "For more information, visit: bitbucket.org/drk4/screen_ruler\n\n"
                   "You can find there a wiki (with the documentation), and an issues tracker,\n"
                   "where you can write suggestions or problems with the application.\n\n"
                   "Thanks for using this program." );

text_ui.set_selectable( true );


    // :: Grid - Container :: //

container_ui.add( text_ui );

    // :: Window :: //

add( container_ui );

set_title( "About" );

set_resizable( false );

set_border_width( 10 );

show_all_children();
}


void About::open()
{
SecondaryWindow::open();

    //when the window was opened, all the text appeared selected.. so this fixes that
text_ui.select_region( 0, 0 );
}
