#include "screen_ruler.h"
#include "main.h"



ScreenRuler::ScreenRuler()

    : m_pMenuPopup( 0 ),
      orientation_var( "left" ),
      units_var( "pixels" )

{
createMenu( this );


add( draw );


    // :: Window :: //

set_title("Screen ruler");

set_decorated( false );


set_default_size(500, 50);


set_size_request(50, 20);
//window.drag_highlight();

set_resizable(true);

show_all_children();

    //for dragging with left click
add_events( Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::KEY_PRESS_MASK );

signal_button_press_event().connect( sigc::mem_fun( *this, &ScreenRuler::buttonPressEvents ) );
signal_key_press_event().connect( sigc::mem_fun( *this, &ScreenRuler::keyboardShortcuts ) );


signal_hide().connect( sigc::mem_fun( *this, &ScreenRuler::whenClosingWindow ) );

    //updates the configurations values from the external file
configurations_var.load();

    // and make the apropriate changes
load();
}




/*
    Save the program's state when we close the window
 */

void ScreenRuler::whenClosingWindow()
{
configurations_var.save();
}



/*
    Create the popup menu (right click to open it)
 */

void ScreenRuler::createMenu(ScreenRuler* ruler)
{
 //Create actions:

  //Fill menu:

  m_refActionGroup = Gtk::ActionGroup::create();

  //File|New sub menu:
  //These menu actions would normally already exist for a main menu, because a
  //context menu should not normally contain menu items that are only available
  //via a context menu.
  m_refActionGroup->add(Gtk::Action::create("ContextMenu", "Context Menu"));

m_refActionGroup->add(Gtk::Action::create("Options", "Options"),
          Gtk::AccelKey("<control>O"),
          sigc::mem_fun(options, &Options::open));


  m_refActionGroup->add( Gtk::Action::create("About", "About"),
          sigc::mem_fun( about, &About::open ) );



  m_refActionGroup->add(Gtk::Action::create("Close", "Close"),
          sigc::mem_fun(*ruler, &Gtk::Window::hide));



  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);

  add_accel_group(m_refUIManager->get_accel_group());

  //Layout the actions in a menubar and toolbar:
  Glib::ustring ui_info =
        "<ui>"
        "  <popup name='PopupMenu'>"
        "    <menuitem action='Options'/>"
        "    <menuitem action='About'/>"
        "    <menuitem action='Close'/>"
        "  </popup>"
        "</ui>";

  try
  {
    m_refUIManager->add_ui_from_string(ui_info);
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

  //Get the menu:
  m_pMenuPopup = dynamic_cast<Gtk::Menu*>(
          m_refUIManager->get_widget("/PopupMenu"));
  if(!m_pMenuPopup)
    g_warning("menu not found");


}



/*
    Change the program in according to the configurations
 */

void ScreenRuler::load()
{
    cout << configurations_var.units << endl;
setUnits( configurations_var.units );
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
    Glib::RefPtr<Gdk::Window> window = get_window();

    if (!window)
        {
        cout << "no window" << endl;

        return true;
        }


    if (orientation_var == "left")
        {
        orientation_var = "up";
        }

    else
        {
        orientation_var = "left";
        }


    int width = window->get_width();
    int height = window->get_height();


    window->move_resize(mouse_beg_x - (mouse_beg_y - win_pos_beg_y), mouse_beg_y - (mouse_beg_x - win_pos_beg_x), height, width);
    }


return true;
}




bool ScreenRuler::on_motion_notify_event(GdkEventMotion* event)
{
const Glib::RefPtr< Gdk::Cursor > cursor = Gdk::Cursor::create(Gdk::HAND2); //HERE qual o cursor do drag?..

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
    //alt + ( o )ptions --> open the options window
if (event->type   == GDK_KEY_PRESS &&
   (event->keyval == GDK_KEY_O || event->keyval == GDK_KEY_o) &&
   (event->state  & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK)
    {
    options.open();

    return true;
    }

return false;
}




void ScreenRuler::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}



std::string ScreenRuler::getOrientation() const
{
return orientation_var;
}


bool ScreenRuler::hasHorizontalOrientation() const  //HERE
{
if (getOrientation() == "left")
    {
    return true;
    }

return false;
}



std::string ScreenRuler::getUnits() const
{
return units_var;
}


void ScreenRuler::setUnits( std::string units )
{
units_var = units;
}
