/*

	Copyright - 2011 - Pedro Ferreira

	This file is part of Screen_ruler.

    Screen_ruler is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Screen_ruler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Screen_ruler.  If not, see <http://www.gnu.org/licenses/>.

 */



#ifndef DRAW_GUARD
#define DRAW_GUARD


#include <gtkmm/drawingarea.h>
#include <pangomm.h>


class Draw : public Gtk::DrawingArea
{
    public:

        Draw();

        double getProportion() const;
        void forceReDraw();

    private:

            //Override default signal handler:
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

        void measureAsText( const Cairo::RefPtr<Cairo::Context>& cr, int i, std::string units, std::string shortUnits, int traceLengthLimit );

            // from how many pixels we start doing something
        int step_var;

            //the proportion of the unit that is set, to pixels
        double proportion_var;


            //dimensions of the window where the DrawingArea is
        int width_var;
        int height_var;


        bool hasHorizontalOrientation_var;


            //only show the unit next to the number sometimes (show/not/show/not....)
            //HERE -- temp variable for measureAsText()
        bool showUnit_var;

};



#endif //DRAW_GUARD
