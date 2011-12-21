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



#ifndef CONFIGURATIONS_GUARD
#define CONFIGURATIONS_GUARD

/*
    To save/load the state of the program
 */

#include <gtkmm.h>

#include <string>
#include <iostream>
#include <fstream>

class Configurations
{
    public:

        Configurations();

            //saves the configurations to a file
        void save();

            //loads from a file
        void load();


            // :: Variables (they're public only to get the value -- don't change outside of this class) :: //

        bool isAlwaysAbove;

        bool isOptionsOpened;

        std::string units;
        bool hasHorizontalOrientation;

        double rulerWidth;
        double rulerHeight;

        Gdk::RGBA backgroundColor;
        Gdk::RGBA numberLinesColor;


        int rulerPosition_x;
        int rulerPosition_y;

        int optionsPosition_x;
        int optionsPosition_y;      //there's also the About window, but there's no need to save its position, since it won't be used alot


    private:

        void loadFromFile();


        template <typename ValueType>
            ValueType getPropertyValue (std::string line, std::string property);


};



#endif // CONFIGURATIONS_GUARD
