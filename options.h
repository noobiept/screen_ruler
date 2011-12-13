#ifndef OPTIONS_GUARD
#define OPTIONS_GUARD


#include <gtkmm.h>


class Options : public Gtk::Window
{
    public:


        Options();


            // open the options window

        void open();



        sigc::signal<void, int> signal_onAlwaysAboveChange();

    private:


        sigc::signal<void, int> the_signal_onAlwaysAboveChange;


            // :: Event functions :: //

        void alwaysAboveEvents();


            // :: Layout :: //

        Gtk::Grid container_ui;

            Gtk::Box firstLine_ui;

                Gtk::Label angleLabel_ui;
                Gtk::SpinButton angle_ui;
                Gtk::CheckButton alwaysAbove_ui;

            Gtk::Box secondLine_ui;

                Gtk::RadioButton pixels_ui;
                Gtk::RadioButton centimeters_ui;

};

#endif // OPTIONS_GUARD
