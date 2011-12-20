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
