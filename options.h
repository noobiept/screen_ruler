#ifndef OPTIONS_GUARD
#define OPTIONS_GUARD


#include <gtkmm.h>
#include "secondary_window.h"
#include "my_spin_button.h"

class Options : public SecondaryWindow
{
    public:


        Options();

        void setAlwaysAbove( bool yesNo );

        void setBackgroundColor( Gdk::RGBA color );

        void setNumberLinesColor( Gdk::RGBA color );

    private:

            // :: Event functions :: //

        void alwaysAboveEvents();

        void onUnitsChange(std::string unit, Gtk::RadioButton* button);

        void backgroundColorEvents();
        void numberLinesColorEvents();

            // :: Layout :: //

        Gtk::Grid container_ui;

            Gtk::Box firstLine_ui;

                Gtk::Label backgroundColorLabel_ui;
                Gtk::ColorButton backgroundColor_ui;

                Gtk::Label numberLinesColorLabel_ui;
                Gtk::ColorButton numberLinesColor_ui;



            Gtk::Box secondLine_ui;

                Gtk::RadioButton pixels_ui;
                Gtk::RadioButton centimeters_ui;
                Gtk::RadioButton inches_ui;

                Gtk::CheckButton alwaysAbove_ui;

};

#endif // OPTIONS_GUARD
