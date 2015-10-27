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



#include "secondary_window.h"
#include "main.h"


SecondaryWindow::SecondaryWindow()

    : isOpened_var( false )

{
    // :::: Events :::: //

this->add_events( Gdk::KEY_RELEASE_MASK );

this->signal_key_release_event().connect ( sigc::mem_fun( *this, &SecondaryWindow::onKeyRelease ) );
}



/*
    shows the window if its closed, or brings it to the front if its already opened
 */

void SecondaryWindow::open()
{
isOpened_var = true;

present();
}



bool SecondaryWindow::isOpened() const
{
return isOpened_var;
}




void SecondaryWindow::on_hide ()
{
isOpened_var = false;

Gtk::Window::on_hide();
}





/*
    Close the window when the 'esc' key is pressed
 */

bool SecondaryWindow::onKeyRelease (GdkEventKey *event)
{
if (event->keyval == GDK_KEY_Escape)
    {
    hide();
    }

return true;
}
