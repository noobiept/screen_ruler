#include "screen_ruler.h"
#include "main.h"



ScreenRuler::ScreenRuler()

    : m_pMenuPopup(0),
      orientation_var("left")

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
add_events( Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK );

signal_button_press_event().connect( sigc::mem_fun( *this, &ScreenRuler::buttonPressEvents ) );
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



bool ScreenRuler::buttonPressEvents(GdkEventButton* event)
{
mouse_beg_x = event->x_root;
mouse_beg_y = event->y_root;

get_position( win_pos_beg_x, win_pos_beg_y );


    //right click -> open the popup menu
if (event->type == GDK_BUTTON_PRESS && event->button == 3)
    {
        if(m_pMenuPopup)
      m_pMenuPopup->popup(event->button, event->time);
    }


    //on middle click, rotate the ruler
else if (event->type == GDK_BUTTON_PRESS && event->button == 2)
    {
cout << "middle click " << endl;


    Glib::RefPtr<Gdk::Window> window = Glib::wrap(event->window);
 //   Glib::RefPtr<Gdk::Window> window = event->window;
    Glib::RefPtr<Gdk::Window> window2 = get_window();

    if (window) //HERE
        {
            cout << "no window" << endl;

            if (window2)
                {
                    cout << "no window2 too" << endl;
                }
            return true;
        }

    int width = window->get_width();
    int height = window->get_height();

cout<<orientation_var<<endl;

    if (orientation_var == "left")
        {
        orientation_var = "up";

cout << "to up" << endl;

                //HERE check se recebeu.. se n eh null
        window->move_resize(width, height, height, width);

        }

    else
        {
        orientation_var = "left";
cout << "to left" << endl;

                //HERE check se recebeu.. se n eh null
        window->move_resize(width, height, height, width);
        }



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



void ScreenRuler::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}
