#ifndef SECONDARY_WINDOW_GUARD
#define SECONDARY_WINDOW_GUARD

#include <gtkmm.h>


class SecondaryWindow : public Gtk::Window
{
    public:

        SecondaryWindow();


        void open();

    private:


        virtual bool onKeyRelease(GdkEventKey *event);


};

#endif // SECONDARY_WINDOW_GUARD
