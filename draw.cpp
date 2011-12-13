#include "draw.h"
#include "main.h"


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


//cr->scale(1.0 / width , 1.0 / height );

cr->set_source_rgba(0.87, 0.83, 0.13, 0.5);    //white
cr->rectangle(0, 0, width, height);
cr->fill();


cr->begin_new_path();


cr->set_line_width(1);
cr->set_source_rgba(0, 0, 0, 1);

double small = height / 6.0;
double medium = height / 4;
double large = height / 3;



    // :: draw the lines on top :: //

double lineHeight = small;

for (int i = 0 ; i < width ; i += step_var)
    {



    if (((i % 100) * step_var) == 0)
        {
        lineHeight = large;


            // draw the text

        std::stringstream stream;

        stream << i << "px";


        Glib::RefPtr<Pango::Layout> textDrawing = Gtk::Widget::create_pango_layout( stream.str() );


        int textHeight = 10;
        textDrawing->set_height(textHeight);

        int textWidth = 50;         //HERE hmmm
        textDrawing->set_width(textWidth);

        cr->move_to( i - textWidth / 2, height / 2 - textHeight );

        textDrawing->show_in_cairo_context(cr);
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineHeight = medium;
        }

    else
        {
        lineHeight = small;
        }

    cr->move_to(i, 0);
    cr->line_to(i, lineHeight);
    }

cr->stroke();






Glib::RefPtr< Gdk::Screen > screen = get_window()->get_screen();

cout << screen->get_width() << " " << screen->get_width_mm() << endl;


/*
    1 mm     -> something pixel
    width mm -> width pixel
 */

double pxToMm = 1.0 * screen->get_width() / screen->get_width_mm();

double pxToCm = pxToMm / 10;
    // :: draw in centimeters :: //

lineHeight = small;

for (int i = 0 ; i < width / pxToCm ; i += step_var )
    {
    if (((i % 100) * step_var) == 0)
        {
        lineHeight = large;
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineHeight = medium;
        }

    else
        {
        lineHeight = small;
        }

    cr->move_to(i * pxToCm, height);
    cr->line_to(i * pxToCm, height - lineHeight);
    }

cr->stroke();



    // :: draw the lines on bottm :: //
/*
lineHeight = small;

for (int i = 0 ; i < width ; i += step_var)
    {
    if (((i % 100) * step_var) == 0)
        {
        lineHeight = large;
        }

    else if (((i % 50) * step_var) == 0)
        {
        lineHeight = medium;
        }

    else
        {
        lineHeight = small;
        }

    cr->move_to(i, height);
    cr->line_to(i, height - lineHeight);
    }

cr->stroke();
*/


return true;
}



