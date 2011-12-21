#ifndef SCREEN_RULER_GUARD
#define SCREEN_RULER_GUARD


#include <gtkmm.h>
#include <string>

#include "draw.h"
#include "options.h"
#include "about.h"


class ScreenRuler : public Gtk::Window
{
    public:

        ScreenRuler();

        void load();


        void setHorizontalOrientation( bool yesNo, bool toMiddleOfScreen = false );
        bool hasHorizontalOrientation() const;


        void setUnits( std::string unit );
        std::string getUnits() const;

        std::string getShortUnits() const;


        void rotate( bool toMiddleOfScreen = false );

        Draw draw;
        Options options;
        About about;


    private:



            // :: Ruler events :: //

        bool buttonPressEvents(GdkEventButton* event);

        virtual bool on_motion_notify_event(GdkEventMotion* event);
        virtual bool on_leave_notify_event(GdkEventMotion* event);

        bool keyboardShortcuts(GdkEventKey* event);


            //override Gtk::Window's function (when we close the program)
        virtual void on_hide();


            // :: Popup menu :: //

        void createMenu( ScreenRuler* ruler );


        Glib::RefPtr<Gtk::UIManager> m_refUIManager;
        Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

        Gtk::Menu* m_pMenuPopup;


        int mouse_beg_x, mouse_beg_y;
        int win_pos_beg_x, win_pos_beg_y;


        bool hasHorizontalOrientation_var;
        std::string units_var;
        std::string shortUnits_var;

};



#endif //SCREEN_RULER_GUARD
