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
