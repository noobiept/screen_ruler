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


#include <gtkmm.h>

#include "main.h"
#include "screen_ruler.h"
#include "configurations.h"

ScreenRuler* SCREEN_RULER;

Configurations CONFIGURATIONS;



int main(int argc, char *argv[])
{
Gtk::Main kit( argc, argv );


    //has to be before ScreenRuler, since this variable is going to be accessed by ScreenRuler
CONFIGURATIONS.load();

SCREEN_RULER = new ScreenRuler;

    //reads the configurations and makes the apropriate changes
SCREEN_RULER->load();


Gtk::Main::run( *SCREEN_RULER );

return EXIT_SUCCESS;
}
