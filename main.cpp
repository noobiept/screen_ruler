#include <iostream>

#include <gtkmm.h>

#include "main.h"
#include "screen_ruler.h"


using namespace std;

int main(int argc, char *argv[])
{
Gtk::Main kit( argc, argv );

ScreenRuler ruler;


Gtk::Main::run( ruler );

return EXIT_SUCCESS;
}
