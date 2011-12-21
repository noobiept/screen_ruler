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



#ifndef SECONDARY_WINDOW_GUARD
#define SECONDARY_WINDOW_GUARD

#include <gtkmm.h>


class SecondaryWindow : public Gtk::Window
{
    public:

        SecondaryWindow();


        virtual void open();


        bool isOpened() const;


    private:


            //override function, that is called when the window is hidden (or closed)
        virtual void on_hide ();

        virtual bool onKeyRelease(GdkEventKey *event);


        bool isOpened_var;

};

#endif // SECONDARY_WINDOW_GUARD
