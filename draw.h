#ifndef DRAW_GUARD
#define DRAW_GUARD


#include <gtkmm/drawingarea.h>
#include <pangomm.h>


class Draw : public Gtk::DrawingArea
{
    public:

        Draw();


    private:

            //Override default signal handler:
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);



        int step_var;


        double smallHeight_var;
        double mediumHeight_var;
        double largeHeight_var;
};



#endif //DRAW_GUARD
