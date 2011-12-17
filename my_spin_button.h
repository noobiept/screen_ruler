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
