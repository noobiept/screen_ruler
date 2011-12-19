#include "draw.h"
#include "main.h"

#include "screen_ruler.h"

extern ScreenRuler* SCREEN_RULER;


Draw::Draw()

    : step_var ( 5 ),    //from each line, how much pixels go

      smallHeight_var( 0 ),     //HERE k valor para aqui
      mediumHeight_var( 0 ),
      largeHeight_var( 0 ),
      hasHorizontalOrientation_var( true )

{

}




bool Draw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
Gtk::Allocation allocation = get_allocation();

width_var = allocation.get_width();
height_var = allocation.get_height();


    // :: The background color :: //

cr->set_source_rgba(0.87, 0.83, 0.13, 0.5);
cr->rectangle(0, 0, width_var, height_var);
cr->fill();




    // :: Find which units to draw the ruler :: //

std::string units = SCREEN_RULER->getUnits();
std::string shortUnits = "px";

    //the proportion of the unit that is set to pixels
double proportion = 1;

Glib::RefPtr< Gdk::Screen > screen = get_window()->get_screen();



if (units == "centimeters")
    {
    shortUnits = "cm";

        // 1 mm     -> something pixel
        // width mm -> width pixel

    double pxToMm = 1.0 * screen->get_width() / screen->get_width_mm();

    double pxToCm = pxToMm / 10;

    proportion = pxToCm;
    }

else if (units == "inches")
    {
    shortUnits = "''";

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


if (SCREEN_RULER->getOrientation() == "left")
    {
    hasHorizontalOrientation_var = true;

    rulerLength = width_var;
    traceLengthLimit = height_var;
    }

    //'up'
else
    {
    hasHorizontalOrientation_var = false;

   // cr->translate(0, 0);
   // cr->rotate_degrees(90); //HERE

    rulerLength = height_var;
    traceLengthLimit = width_var;
    }



    // the length of the traces (lines)
double small = traceLengthLimit / 6.0;
double medium = traceLengthLimit / 4.0;
double large = traceLengthLimit / 3.0;



int limit = rulerLength / proportion;



    // :: draw the lines on top (or right/left?...) :: //

cr->begin_new_path();


cr->set_line_width(0.5);
cr->set_source_rgba(0, 0, 0, 1);



double lineLength = small;


    //reset this variable
showUnit_var = false;

    // :: draw the top line :: //

for (int i = 0 ; i < limit ; i += step_var)
    {
    if (((i % 100) * step_var) == 0)
        {
        lineLength = large;

        measureAsText( cr, i, units, shortUnits, proportion );
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineLength = medium;
        }

    else
        {
        lineLength = small;
        }


    if (hasHorizontalOrientation_var == true)
        {
        cr->move_to(i * proportion, 0);
        cr->line_to(i * proportion, lineLength);
        }

    else
        {
        cr->move_to(0, i * proportion);
        cr->line_to(lineLength, i * proportion);
        }

    //cr->stroke();
    }

cr->stroke();







    // :: draw in centimeters :: //


    // :: Draw the bottom (or right/left.. //HERE ) traces of the ruler :: //

for (int i = 0 ; i < limit ; i += step_var )
    {
    if (((i % 100) * step_var) == 0)
        {
        lineLength = large;
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineLength = medium;
        }

    else
        {
        lineLength = small;
        }

    //HERE
    if (hasHorizontalOrientation_var == true)
        {
        cr->move_to(i * proportion, traceLengthLimit);
        cr->line_to(i * proportion, traceLengthLimit - lineLength);
        }

    else
        {
        cr->move_to(traceLengthLimit, i * proportion);
        cr->line_to(traceLengthLimit - lineLength, i * proportion);
        }

    }

cr->stroke();



if (hasHorizontalOrientation_var == false)
    {
    //int pi = 3.14;

    cr->rotate_degrees(110);
    cr->stroke();
    }



return true;
}


/*
        //HERE nao ter k passar tantos argumentos... por coisas na classe
 */

void Draw::measureAsText( const Cairo::RefPtr<Cairo::Context>& cr, int i, std::string units, std::string shortUnits, double proportion )
{
    // draw the text

std::stringstream stream;

if (units == "pixels")
    {
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


//Glib::RefPtr<Pango::Layout> textDrawing = Gtk::Widget::create_pango_layout( stream.str() );
Glib::RefPtr<Pango::Context> context = Gtk::Widget::create_pango_context();


if (hasHorizontalOrientation_var == false)
    {
    //Glib::RefPtr< Pango::Context > context = textDrawing->get_context();

    context->set_base_gravity( Pango::GRAVITY_EAST );

    //double pi = 3.14;

    //cr->rotate(pi / 2);
    }


Glib::RefPtr<Pango::Layout> textDrawing = Pango::Layout::create( context );

textDrawing->set_text( stream.str() );

int textHeight, textWidth;

    //get the text dimensions (it updates the variables above -- by reference)
textDrawing->get_pixel_size(textWidth, textHeight);

//cout << textWidth << " " << textHeight << endl;
//cout << "i " << i << " textWidth " << textWidth  << " x_position " << i - textWidth / 2 << endl;


    //place the text in the center of the ruler, and also centered with the line mark
if (hasHorizontalOrientation_var == true)
    {
    cr->move_to( i * proportion - textWidth / 2, height_var / 2 - textHeight / 2 );
    }

else
    {
    cr->move_to( width_var / 2 - textWidth / 2, i * proportion - textHeight / 2 );
    }


textDrawing->show_in_cairo_context(cr);
}




void Draw::forceReDraw()
{
    // force our program to redraw the entire image
Glib::RefPtr<Gdk::Window> win = get_window();

if (win)
    {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());

    win->invalidate_rect(r, false);
    }
}


