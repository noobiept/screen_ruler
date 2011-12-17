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
