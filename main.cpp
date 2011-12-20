#include <iostream>

#include <gtkmm.h>

#include "main.h"
#include "screen_ruler.h"
#include "configurations.h"

ScreenRuler* SCREEN_RULER;

Configurations CONFIGURATIONS;



using namespace std;

int main(int argc, char *argv[])
{
Gtk::Main kit( argc, argv );


    //has to be before ScreenRuler, since this variable is going to be accessed by ScreenRuler
CONFIGURATIONS.load();

SCREEN_RULER = new ScreenRuler;


Gtk::Main::run( *SCREEN_RULER );

return EXIT_SUCCESS;
}
