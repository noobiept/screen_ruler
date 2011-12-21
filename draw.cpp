#include "draw.h"
#include "main.h"

#include "screen_ruler.h"
#include "configurations.h"


extern ScreenRuler* SCREEN_RULER;

extern Configurations CONFIGURATIONS;


Draw::Draw()

    : step_var ( 5 ),    //from each line, how much pixels go
      hasHorizontalOrientation_var( true )

{

}




bool Draw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
Gtk::Allocation allocation = get_allocation();

width_var = allocation.get_width();
height_var = allocation.get_height();


    // :: The background color :: //

Gdk::RGBA backgroundColor = CONFIGURATIONS.backgroundColor;

cr->set_source_rgba(
                    backgroundColor.get_red(),
                    backgroundColor.get_green(),
                    backgroundColor.get_blue(),
                    backgroundColor.get_alpha() );

cr->rectangle(0, 0, width_var, height_var);
cr->fill();




    // :: Find which units to draw the ruler :: //

std::string units = SCREEN_RULER->getUnits();
std::string shortUnits = SCREEN_RULER->getShortUnits();

    //the proportion of the unit that is set to pixels
double proportion = 1;

Glib::RefPtr< Gdk::Screen > screen = get_window()->get_screen();



if (units == "centimeters")
    {
        // 1 mm     -> something pixel
        // width mm -> width pixel

    double pxToMm = 1.0 * screen->get_width() / screen->get_width_mm();

    double pxToCm = pxToMm / 10;

    proportion = pxToCm;
    }

else if (units == "inches")
    {
        // we'll calculate inches from centimeters
    double pxToMm = 1.0 * screen->get_width() / screen->get_width_mm();

    double pxToCm = pxToMm / 10;

        // 1 inch -> 2.54 cm
    double pxToInch = 2.54 * pxToCm;

    proportion = pxToInch;
    }



    //the length that we need to draw the lines/traces... its or the window's width or the height, depending on the orientation
int rulerLength;

    //the limit of the traces, from where we calculate the small/medium/large lengths
int traceLengthLimit;




    // ::  Determine the orientation :: //


if (SCREEN_RULER->hasHorizontalOrientation() == true)
    {
    hasHorizontalOrientation_var = true;

    rulerLength = width_var;
    traceLengthLimit = height_var;
    }

    //'up'
else
    {
    hasHorizontalOrientation_var = false;


        //rotate the image
    cr->translate( width_var, 0 );
    cr->rotate_degrees( 90 );

    rulerLength = height_var;
    traceLengthLimit = width_var;
    }



    // the length of the traces (lines)
double small = traceLengthLimit / 6.0;
double medium = traceLengthLimit / 4.0;
double large = traceLengthLimit / 3.0;



int limit = rulerLength / proportion;



    // :: draw the lines on top/right :: //

cr->begin_new_path();


cr->set_line_width( 1 );

//cr->set_antialias( Cairo::ANTIALIAS_GRAY );

Gdk::RGBA numberLinesColor = CONFIGURATIONS.numberLinesColor;

cr->set_source_rgba(
                    numberLinesColor.get_red(),
                    numberLinesColor.get_green(),
                    numberLinesColor.get_blue(),
                    numberLinesColor.get_alpha() );



double lineLength = small;


    //reset this variable
showUnit_var = false;

    // :: draw the top line :: //

for (int i = 0 ; i < limit ; i += step_var)
    {
    if (((i % 100) * step_var) == 0)
        {
        lineLength = large;

        measureAsText( cr, i, units, shortUnits, proportion, traceLengthLimit );
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineLength = large;
        }

    else if (((i % 25) * step_var) == 0)
        {
        lineLength = medium;
        }

    else
        {
        lineLength = small;
        }


    cr->move_to(i * proportion, 0);
    cr->line_to(i * proportion, lineLength);
    }

cr->stroke();




    // :: Draw the bottom/left traces of the ruler :: //

for (int i = 0 ; i < limit ; i += step_var )
    {
    if (((i % 100) * step_var) == 0)
        {
        lineLength = large;
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineLength = large;
        }

    else if (((i % 25) * step_var) == 0)
        {
        lineLength = medium;
        }

    else
        {
        lineLength = small;
        }


    cr->move_to(i * proportion, traceLengthLimit);
    cr->line_to(i * proportion, traceLengthLimit - lineLength);
    }

cr->stroke();




return true;
}




/*
        //HERE nao ter k passar tantos argumentos... por coisas na classe
 */

void Draw::measureAsText( const Cairo::RefPtr<Cairo::Context>& cr, int i, std::string units, std::string shortUnits, double proportion, int traceLengthLimit )
{
    // draw the text

std::stringstream stream;

if (units == "pixels")
    {
        //only show the units every two numbers
    if (showUnit_var == false)
        {
        showUnit_var = true;

        stream << i;
        }

    else
        {
        showUnit_var = false;

        stream << i << shortUnits;
        }
    }

else
    {
    if (showUnit_var == false)
        {
        showUnit_var = true;

        stream << i / 100;
        }

    else
        {
        showUnit_var = false;

        stream << i / 100 << shortUnits;
        }
    }


Glib::RefPtr<Pango::Layout> textDrawing = Gtk::Widget::create_pango_layout( stream.str() );


int textHeight, textWidth;

    //get the text dimensions (it updates the variables above -- by reference)
textDrawing->get_pixel_size( textWidth, textHeight );


cr->move_to( i * proportion - textWidth / 2, traceLengthLimit / 2 - textHeight / 2 );


textDrawing->show_in_cairo_context(cr);
}



/*
    force our program to redraw the entire image (useful when changing the options, for example)
 */

void Draw::forceReDraw()
{
Glib::RefPtr<Gdk::Window> win = get_window();

if (win)
    {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());

    win->invalidate_rect(r, false);
    }
}


