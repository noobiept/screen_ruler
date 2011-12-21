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




#include "screen_ruler.h"
#include "main.h"
#include "configurations.h"


extern Configurations CONFIGURATIONS;


ScreenRuler::ScreenRuler()

    : m_pMenuPopup( 0 ),
      hasHorizontalOrientation_var( true ),
      units_var( "pixels" ),
      shortUnits_var( "px" )

{
createMenu( this );


add( draw );


    // :: Window :: //

set_title("Screen ruler");

set_decorated( false );


set_default_size(500, 50);


set_size_request(50, 20);

set_resizable(true);

show_all_children();


    // :: Events :: //

add_events( Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::KEY_PRESS_MASK );

signal_button_press_event().connect( sigc::mem_fun( *this, &ScreenRuler::buttonPressEvents ) );
signal_key_press_event().connect( sigc::mem_fun( *this, &ScreenRuler::keyboardShortcuts ) );
}




/*
    Save the program's state when we close the window
 */

void ScreenRuler::on_hide()
{
CONFIGURATIONS.save();

Gtk::Window::on_hide();
}



/*
    Create the popup menu (right click to open it)
 */

void ScreenRuler::createMenu(ScreenRuler* ruler)
{
m_refActionGroup = Gtk::ActionGroup::create();

m_refActionGroup->add( Gtk::Action::create( "ContextMenu", "Context Menu" ) );


m_refActionGroup->add( Gtk::Action::create( "Options", "Options"), sigc::mem_fun( options, &Options::open ) );
m_refActionGroup->add( Gtk::Action::create( "Rotate" , "Rotate" ), sigc::bind< bool >( sigc::mem_fun( *ruler , &ScreenRuler::rotate ), false) );
m_refActionGroup->add( Gtk::Action::create( "About"  , "About"  ), sigc::mem_fun( about  , &About::open ) );
m_refActionGroup->add( Gtk::Action::create( "Close"  , "Close"  ), sigc::mem_fun( *ruler , &Gtk::Window::hide ) );


m_refUIManager = Gtk::UIManager::create();
m_refUIManager->insert_action_group( m_refActionGroup );

add_accel_group( m_refUIManager->get_accel_group() );

  //Layout the actions in a menubar and toolbar:
Glib::ustring ui_info =
        "<ui>"
        "  <popup name='PopupMenu'>"
        "    <menuitem action='Options'/>"
        "    <menuitem action='Rotate'/>"
        "    <menuitem action='About'/>"
        "    <menuitem action='Close'/>"
        "  </popup>"
        "</ui>";

try
    {
    m_refUIManager->add_ui_from_string(ui_info);
    }

catch (const Glib::Error& ex)
    {
    std::cerr << "building menus failed: " <<  ex.what();
    }


    //Get the menu:
m_pMenuPopup = dynamic_cast< Gtk::Menu* >( m_refUIManager->get_widget( "/PopupMenu" ) );

if(!m_pMenuPopup)
    {
    g_warning("menu not found");
    }

}



/*
    Change the program in according to the configurations
 */

void ScreenRuler::load()
{
    // :: Units :: //

setUnits( CONFIGURATIONS.units );


    // :: open options window :: //

if ( CONFIGURATIONS.isOptionsOpened == true )
    {
    options.open();

        //we want this window to have precedence (to be in front)
    present();
    }


    // :: Window always above :: //

if ( CONFIGURATIONS.isAlwaysAbove == true )
    {
    options.setAlwaysAbove( true );
    }


    // :: Ruler orientation :: //

if ( CONFIGURATIONS.hasHorizontalOrientation == false )
    {
    setHorizontalOrientation( false );
    }


    // :: Ruler width and height :: //

resize( CONFIGURATIONS.rulerWidth, CONFIGURATIONS.rulerHeight );


    // :: Background color :: //

options.setBackgroundColor( CONFIGURATIONS.backgroundColor );

    // :: Numbers and lines color :: //

options.setNumberLinesColor( CONFIGURATIONS.numberLinesColor );


    // :: Ruler window position :: //

if ( CONFIGURATIONS.rulerPosition_x >= 0 && CONFIGURATIONS.rulerPosition_y >= 0 )
    {
    move( CONFIGURATIONS.rulerPosition_x, CONFIGURATIONS.rulerPosition_y );
    }

    // :: Options window position :: //

if ( CONFIGURATIONS.optionsPosition_x >= 0 && CONFIGURATIONS.optionsPosition_y >= 0 )
    {
    options.move( CONFIGURATIONS.optionsPosition_x, CONFIGURATIONS.optionsPosition_y );
    }
}




/*
    Rotates the ruler 90 degrees
 */

void ScreenRuler::rotate( bool toMiddleOfScreen )
{
    //change to the other orientation (of whatever is set)
options.setOrientation( !hasHorizontalOrientation(), toMiddleOfScreen );
}




/*
    Middle click --> rotate the ruler 90 degrees
    Right click  --> open the popup menu
 */

bool ScreenRuler::buttonPressEvents(GdkEventButton* event)
{
mouse_beg_x = event->x_root;
mouse_beg_y = event->y_root;

get_position( win_pos_beg_x, win_pos_beg_y );


    //right click -> open the popup menu
if (event->type == GDK_BUTTON_PRESS && event->button == 3)
    {
    if(m_pMenuPopup)
        {
        m_pMenuPopup->popup(event->button, event->time);
        }

    }


    //on middle click, rotate the ruler
else if (event->type == GDK_BUTTON_PRESS && event->button == 2)
    {
        //change to the other orientation (of whatever is set)
    rotate();
    }


return true;
}




/*
    Change the cursor of the mouse to show that the ruler is draggable

    Drag the window when we're clicking (and moving)
 */

bool ScreenRuler::on_motion_notify_event(GdkEventMotion* event)
{
    //creating the drag cursor
const Glib::RefPtr< Gdk::Cursor > cursor = Gdk::Cursor::create( Gdk::FLEUR );   //fleur?? wtf

Glib::RefPtr<Gdk::Window> window = get_window();

if (window)
    {
    window->set_cursor(cursor);
    }


   //when clicking and moving the mouse, we drag the ruler
if (event->state & GDK_BUTTON1_MASK)
    {
    int x = win_pos_beg_x + ( event->x_root - mouse_beg_x );
    int y = win_pos_beg_y + ( event->y_root - mouse_beg_y );

    move( x, y );
    }



return true;
}


/*
    Get the normal cursor back
 */

bool ScreenRuler::on_leave_notify_event(GdkEventMotion* event)
{
Glib::RefPtr<Gdk::Window> window = get_window();

if (window)
    {
    window->set_cursor();
    }

return true;
}



/*
    alt + ( o )ptions --> open the options window
 */

bool ScreenRuler::keyboardShortcuts(GdkEventKey* event)
{
if ( event->type == GDK_KEY_PRESS )
    {
        //alt + ( o )ptions --> open the options window
    if ( (event->keyval == GDK_KEY_O || event->keyval == GDK_KEY_o) &&
         (event->state  & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK )
        {
        options.open();

        return true;
        }

        //alt + ( r )otate --> rotate the ruler window
    else if ( (event->keyval == GDK_KEY_R || event->keyval == GDK_KEY_r) &&
         (event->state  & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK )
        {
        rotate( true );

        return true;
        }
    }



return false;
}






bool ScreenRuler::hasHorizontalOrientation() const
{
return hasHorizontalOrientation_var;
}




void ScreenRuler::setHorizontalOrientation( bool yesNo, bool toMiddleOfScreen )
{
Glib::RefPtr<Gdk::Window> window = get_window();

if (!window)
    {
    std::cout << "no window" << std::endl;

    return;
    }


    //its already in the right position
if ( yesNo == hasHorizontalOrientation_var )
    {
    return;
    }

hasHorizontalOrientation_var = yesNo;


int width = window->get_width();
int height = window->get_height();

if ( toMiddleOfScreen == true )
    {
    Glib::RefPtr< const Gdk::Screen > screen = get_screen();

        //get screen dimensions
    int screenWidth = screen->get_width();
    int screenHeight = screen->get_height();

    window->move_resize( screenWidth / 2 - height / 2, screenHeight / 2 - width / 2, height, width );
    }

    //calculate based on where the mouse is
else
    {
    window->move_resize(mouse_beg_x - (mouse_beg_y - win_pos_beg_y), mouse_beg_y - (mouse_beg_x - win_pos_beg_x), height, width);
    }

}



std::string ScreenRuler::getUnits() const
{
return units_var;
}


std::string ScreenRuler::getShortUnits() const
{
return shortUnits_var;
}


void ScreenRuler::setUnits( std::string units )
{
units_var = units;


if (units == "inches")
    {
    shortUnits_var = "''";
    }

else if (units == "pixels")
    {
    shortUnits_var = "px";
    }

else
    {
    shortUnits_var = "cm";
    }
}


