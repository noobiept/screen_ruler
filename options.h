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

        void setOrientation( bool horizontalOrientation, bool toMiddleOfScreen = false );

        void setBackgroundColor( Gdk::RGBA color );

        void setNumberLinesColor( Gdk::RGBA color );

    private:

            // :: Event functions :: //

        void alwaysAboveEvents();

        void onUnitsChange(std::string unit, Gtk::RadioButton* button);

        void onOrientationChange( bool horizontalOrientation, Gtk::RadioButton* button );

        void backgroundColorEvents();
        void numberLinesColorEvents();

            // :: Layout :: //

        Gtk::Grid container_ui;

                // units column
            Gtk::RadioButton pixels_ui;
            Gtk::RadioButton centimeters_ui;
            Gtk::RadioButton inches_ui;

                // orientation column
            Gtk::Label orientation_ui;
            Gtk::RadioButton horizontal_ui;
            Gtk::RadioButton vertical_ui;

                // color (and alwaysAbove) column
            Gtk::Box backgroundColorContainer_ui;

                Gtk::Label backgroundColorLabel_ui;
                Gtk::ColorButton backgroundColor_ui;

            Gtk::Box numberLinesColorContainer_ui;

                Gtk::Label numberLinesColorLabel_ui;
                Gtk::ColorButton numberLinesColor_ui;

            Gtk::CheckButton alwaysAbove_ui;


};

#endif // OPTIONS_GUARD
