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
backgroundColor.set_rgba( 0.87, 0.83, 0.13, 0.5 );
numberLinesColor.set_rgba( 0, 0, 0, 1 );
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



    config << "backgroundColor-red: "   << backgroundColor.get_red()   << "\n";
    config << "backgroundColor-green: " << backgroundColor.get_green() << "\n";
    config << "backgroundColor-blue: "  << backgroundColor.get_blue()  << "\n";
    config << "backgroundColor-alpha: " << backgroundColor.get_alpha() << "\n";

    config << "numberLinesColor-red: "   << numberLinesColor.get_red()   << "\n";
    config << "numberLinesColor-green: " << numberLinesColor.get_green() << "\n";
    config << "numberLinesColor-blue: "  << numberLinesColor.get_blue()  << "\n";
    config << "numberLinesColor-alpha: " << numberLinesColor.get_alpha() << "\n";


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


        // :: background color :: //

    int red, green, blue, alpha;

    getline( config, line );
    red = getPropertyValue< double >( line, "backgroundColor-red" );

    getline( config, line );
    green = getPropertyValue< double >( line, "backgroundColor-green" );

    getline( config, line );
    blue = getPropertyValue< double >( line, "backgroundColor-blue" );

    getline( config, line );
    alpha = getPropertyValue< double >( line, "backgroundColor-alpha" );


    this->backgroundColor.set_rgba( red, green, blue, alpha );


        // number and lines color :: //

    getline( config, line );
    red = getPropertyValue< double >( line, "numberLinesColor-red" );

    getline( config, line );
    green = getPropertyValue< double >( line, "numberLinesColor-green" );

    getline( config, line );
    blue = getPropertyValue< double >( line, "numberLinesColor-blue" );

    getline( config, line );
    alpha = getPropertyValue< double >( line, "numberLinesColor-alpha" );


    this->numberLinesColor.set_rgba( red, green, blue, alpha );


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
