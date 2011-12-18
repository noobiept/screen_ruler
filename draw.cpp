#include "draw.h"
#include "main.h"

#include "screen_ruler.h"

extern ScreenRuler* SCREEN_RULER;


Draw::Draw()

    : step_var ( 5 ),    //from each line, how much pixels go

      smallHeight_var( 0 ),     //HERE k valor para aqui
      mediumHeight_var( 0 ),
      largeHeight_var( 0 )

{

}




bool Draw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
Gtk::Allocation allocation = get_allocation();

const int width = allocation.get_width();
const int height = allocation.get_height();


    // :: The background color :: //

cr->set_source_rgba(0.87, 0.83, 0.13, 0.5);
cr->rectangle(0, 0, width, height);
cr->fill();


    // ::  Determine the orientation :: //


bool leftOrientation = true;    //HERE hasLeftOrientation



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

if (SCREEN_RULER->getOrientation() == "left")
    {
    leftOrientation = true;

    rulerLength = width;
    traceLengthLimit = height;
    }

    //'up'
else
    {
    leftOrientation = false;

    rulerLength = height;
    traceLengthLimit = width;
    }



    // the length of the traces (lines)
double small = traceLengthLimit / 6.0;
double medium = traceLengthLimit / 4.0;
double large = traceLengthLimit / 3.0;



int limit = rulerLength / proportion;

    // :: draw the lines on top (or right/left?...) :: //

cr->begin_new_path();


cr->set_line_width(1);
cr->set_source_rgba(0, 0, 0, 1);



double lineLength = small;

for (int i = 0 ; i < limit ; i += step_var)
    {
    if (((i % 100) * step_var) == 0)
        {
        lineLength = large;



            // draw the text

        std::stringstream stream;

        stream << i << shortUnits;


        //Glib::RefPtr<Pango::Layout> textDrawing = Gtk::Widget::create_pango_layout( stream.str() );
Glib::RefPtr<Pango::Context> context = Gtk::Widget::create_pango_context();


        if (leftOrientation == false)
            {
            //Glib::RefPtr< Pango::Context > context = textDrawing->get_context();

            context->set_base_gravity( Pango::GRAVITY_EAST );

            //double pi = 3.14;

            //cr->rotate(pi / 2);
            }


Glib::RefPtr<Pango::Layout> textDrawing = Pango::Layout::create( context );


textDrawing->set_text( stream.str() );

        int textHeight = 10;
        //textDrawing->set_height(textHeight);

        int textWidth = 50;         //HERE hmmm
        //textDrawing->set_width(textWidth);


        if (leftOrientation == true)
            {
            cr->move_to( i - textWidth / 2, height / 2 - textHeight );
            }

        else
            {
            cr->move_to( width / 2 - textHeight, i - textWidth );
            }




        textDrawing->show_in_cairo_context(cr);
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineLength = medium;
        }

    else
        {
        lineLength = small;
        }


    if (leftOrientation == true)
        {
        cr->move_to(i * proportion, 0);
        cr->line_to(i * proportion, lineLength);
        }

    else
        {
        cr->move_to(0, i * proportion);
        cr->line_to(lineLength, i * proportion);
        }
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
    if (leftOrientation == true)
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



if (leftOrientation == false)
    {
    //int pi = 3.14;

    cr->rotate_degrees(110);
    cr->stroke();
    }



return true;
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


