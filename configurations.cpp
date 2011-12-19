#include "configurations.h"

#include "screen_ruler.h"
#include "main.h"

extern ScreenRuler* SCREEN_RULER;



Configurations::Configurations()

    : isAlwaysAbove( false ),

      isOptionsOpened( false ),

      units( "pixels" ),
      hasHorizontalOrientation( true ),

      rulerWidth( 500 ),
      rulerHeight( 50 )

{

}




void Configurations::save()
{
std::ofstream config;

config.open ("config.txt", std::ios::out | std::ios::trunc);

if (config.is_open() == true)
    {
    config << "isAlwaysAbove: " << isAlwaysAbove << "\n";

    config << "isOptionsOpened: " << SCREEN_RULER->options.property_visible() << "\n";      //HERE ou get_visible() ?..

    config << "units: " << SCREEN_RULER->getUnits() << "\n";
    config << "hasHorizontalOrientation: " << SCREEN_RULER->hasHorizontalOrientation() << "\n";

    double width = SCREEN_RULER->get_width();
    double height = SCREEN_RULER->get_height();

    config << "rulerWidth: " << width << "\n";
    config << "rulerHeight: " << height << "\n";

    config.close ();
    }

else
    {
    std::cout << "failed opening the config.txt file\n";
    }
}


void Configurations::load()
{
try
    {
    loadFromFile();
    }

catch (...)
    {
    std::cout << "exception occurred, when trying to read from the configurations file ( Configurations::load() )\n";
    }
}



void Configurations::loadFromFile()
{
std::ifstream config;

config.open ("config.txt");

if (config.is_open() == true)
    {
    std::string line;


        // :: is always above :: //

    getline (config, line);

    this->isAlwaysAbove = getPropertyValue< bool >(line, "isAlwaysAbove");


        // :: is options opened :: //

    getline( config, line );

    this->isOptionsOpened = getPropertyValue< bool >(line, "isOptionsOpened");


        // :: units :: //

    getline( config, line );

    this->units = getPropertyValue< std::string >( line, "units" );


        // :: has horizontal orientation :: //

    getline( config, line );

    this->hasHorizontalOrientation = getPropertyValue< bool >( line, "hasHorizontalOrientation" );


        // :: ruler width :: //

    getline( config, line );

    this->rulerWidth = getPropertyValue< double >( line, "rulerWidth" );


        // :: ruler height :: //

    getline( config, line );

    this->rulerHeight = getPropertyValue< double >( line, "rulerHeight" );


    config.close();
    }

}





/*
    Searches a string for a property, and returns that property's value
        (in the form -> property: value)

 */

template <typename ValueType>

ValueType Configurations::getPropertyValue (std::string line, std::string property)
{
int i = line.find (property);


i += property.length();

    //remove the ':' and spaces
while (true)
    {
    if (line.at (i) != ':' && line.at (i) != ' ')
        {
        break;
        }

    i++;
    }

std::string theNumber = line.substr (i);


std::stringstream stream (theNumber);

ValueType value;

stream >> value;

    //abort the loading process
if (stream.fail() == true)
    {
    throw -1;
    }

return value;
}
