#ifndef DRAW_GUARD
#define DRAW_GUARD


#include <gtkmm/drawingarea.h>
#include <pangomm.h>


class Draw : public Gtk::DrawingArea
{
    public:

        Draw();


        void forceReDraw();

    private:

            //Override default signal handler:
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

        void measureAsText( const Cairo::RefPtr<Cairo::Context>& cr, int i, std::string units, std::string shortUnits, double proportion );


        int step_var;


        double smallHeight_var;
        double mediumHeight_var;
        double largeHeight_var;

            //dimensions of the window where the DrawingArea is
        int width_var;
        int height_var;


        bool hasHorizontalOrientation_var;


            //only show the unit next to the number sometimes (show/not/show/not....)
            //HERE -- temp variable for measureAsText()
        bool showUnit_var;

};



#endif //DRAW_GUARD
