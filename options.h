#ifndef OPTIONS_GUARD
#define OPTIONS_GUARD


#include <gtkmm.h>
#include "secondary_window.h"
#include "my_spin_button.h"

class Options : public SecondaryWindow
{
    public:


        Options();


    private:

            // :: Event functions :: //

        void alwaysAboveEvents();

        void onUnitsChange(std::string unit);


            // :: Layout :: //

        Gtk::Grid container_ui;

            Gtk::Box firstLine_ui;

                Gtk::Label angleLabel_ui;
                MySpinButton angle_ui;
                Gtk::CheckButton alwaysAbove_ui;

            Gtk::Box secondLine_ui;

                Gtk::RadioButton pixels_ui;
                Gtk::RadioButton centimeters_ui;
                Gtk::RadioButton inches_ui;

};

#endif // OPTIONS_GUARD
