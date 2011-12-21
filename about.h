#ifndef ABOUT_GUARD
#define ABOUT_GUARD


#include <gtkmm.h>
#include "secondary_window.h"

class About : public SecondaryWindow
{
    public:

        About();

        void open();

    private:

        Gtk::Grid container_ui;

            Gtk::Label text_ui;

};

#endif // ABOUT_GUARD
