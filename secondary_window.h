#ifndef SECONDARY_WINDOW_GUARD
#define SECONDARY_WINDOW_GUARD

#include <gtkmm.h>


class SecondaryWindow : public Gtk::Window
{
    public:

        SecondaryWindow();


        void open();


        bool isOpened() const;


    private:


            //override function, that is called when the window is hidden (or closed)
        virtual void on_hide ();

        virtual bool onKeyRelease(GdkEventKey *event);


        bool isOpened_var;

};

#endif // SECONDARY_WINDOW_GUARD
