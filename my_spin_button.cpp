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



#include "my_spin_button.h"



bool MySpinButton::on_key_release_event (GdkEventKey* event)
{
    //call base function
Gtk::SpinButton::on_key_release_event(event);

    //when pressing the 'esc' key
if (event->keyval == GDK_KEY_Escape)
    {
        //let the event propagate (to, eventually, the SecondaryWindow, that will close the window)
    return false;
    }


return true;
}
