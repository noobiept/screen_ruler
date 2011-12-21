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



#ifndef MY_SPIN_BUTTON_GUARD
#define MY_SPIN_BUTTON_GUARD

#include <gtkmm.h>


/*

    The Gtk::SpinButton doesn't emit the signal on key release (only on key press)

    So, this class deals with that, the only difference is that it checks for the 'esc' key on the key release
        event, and allows the event to propagate if that happens.

    It will eventually reach the SecundaryWindow, which will close the window

    Its only needed for the Gtk::SpinButton of the SecundaryWindow

*/

class MySpinButton : public Gtk::SpinButton
{
    private:

        virtual bool on_key_release_event (GdkEventKey* event);
};



#endif //MY_SPIN_BUTTON_GUARD
