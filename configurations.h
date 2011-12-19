#ifndef CONFIGURATIONS_GUARD
#define CONFIGURATIONS_GUARD

/*
    To save/load the state of the program
 */


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


        bool isAlwaysAbove;

        bool isOptionsOpened;

        std::string units;
        bool hasHorizontalOrientation;

        double rulerWidth;
        double rulerHeight;



    private:

        void loadFromFile();


        template <typename ValueType>
            ValueType getPropertyValue (std::string line, std::string property);


};



#endif // CONFIGURATIONS_GUARD
