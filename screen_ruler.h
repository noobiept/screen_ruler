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


        bool hasHorizontalOrientation() const;
        std::string getOrientation() const; //HERE substitute for hasHorizontalOrientation()


        void setUnits( std::string unit );
        std::string getUnits() const;

        Draw draw;
        Options options;
        About about;


    private:



            // :: Ruler events :: //

        bool buttonPressEvents(GdkEventButton* event);

        virtual bool on_motion_notify_event(GdkEventMotion* event);
        virtual bool on_leave_notify_event(GdkEventMotion* event);

        bool keyboardShortcuts(GdkEventKey* event);




            // :: Popup menu :: //

        void createMenu( ScreenRuler* ruler );


        Glib::RefPtr<Gtk::UIManager> m_refUIManager;
        Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

        Gtk::Menu* m_pMenuPopup;
        void on_menu_file_popup_generic();


        void load();

        void whenClosingWindow();


        int mouse_beg_x, mouse_beg_y;
        int win_pos_beg_x, win_pos_beg_y;



        std::string orientation_var;
        std::string units_var;

};



#endif //SCREEN_RULER_GUARD
