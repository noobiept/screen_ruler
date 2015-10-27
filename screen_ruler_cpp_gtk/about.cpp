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
