#include <iostream>

#include <gtkmm.h>

#include "main.h"
#include "screen_ruler.h"


ScreenRuler* SCREEN_RULER;



using namespace std;

int main(int argc, char *argv[])
{
Gtk::Main kit( argc, argv );

SCREEN_RULER = new ScreenRuler;


Gtk::Main::run( *SCREEN_RULER );

return EXIT_SUCCESS;
}
