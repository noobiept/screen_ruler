#include "secondary_window.h"



SecondaryWindow::SecondaryWindow()
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
present();
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
