#include "about.h"



About::About()
{
text_ui.set_label( "Link: www.bitbucket.org/drk4/screen_ruler (probably)" ) ;


    // :: Grid - Container :: //

container_ui.add( text_ui );

    // :: Window :: //

add( container_ui );

set_title( "About" );

set_resizable( false );

set_border_width( 10 );

show_all_children();
}
