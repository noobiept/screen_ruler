/*

	Copyright - 2011 - Pedro Ferreira

	This file is part of Screen_ruler.

    Screen_ruler is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Screen_ruler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Screen_ruler.  If not, see <http://www.gnu.org/licenses/>.

 */



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

        void setMouseLength( int pixelLength );

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

                // orientation/alwaysAbove column
            Gtk::CheckButton alwaysAbove_ui;
            Gtk::RadioButton horizontal_ui;
            Gtk::RadioButton vertical_ui;

                // color/mouseLength column

            Gtk::Box backgroundColorContainer_ui;

                Gtk::Label backgroundColorLabel_ui;
                Gtk::ColorButton backgroundColor_ui;

            Gtk::Box numberLinesColorContainer_ui;

                Gtk::Label numberLinesColorLabel_ui;
                Gtk::ColorButton numberLinesColor_ui;

            Gtk::Label mouseLength_ui;

};

#endif // OPTIONS_GUARD
