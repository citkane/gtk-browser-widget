[Home](../../#development)<br>
[Development notes](..)

# Gtk
This folder is for integrations and presentation of the browser engine window into a `Gtk::Window`

## Browser_window.hh
Provides the top level `Gtk::Window` instance that will wrap the native browser engine window.

## Browser_engine.hh
Header file that uses pragma guards to include the relevant browser engine class header. The included browser header file MUST alias it's class to `gbw::browser::Browser_engine`

## Class Hierarchy
```mermaid
classDiagram-v2
    namespace gbw_custom_Gtk {
      class Browser_widget["gbw::Browser_widget"]{
          +sigc::signal ready()
          +Browser::apis get_browser_apis()
          #Gtk::Widget.methods()
          -Browser_engine browser_engine
      }

      class Browser_window["gbw::gtk::Browser_window"]{
        #Gtk::Window methods()
        #gbw::gtk common()
        #OS impls()
        #virtual::Browser declarations()*
      }
    }

    class Browser_engine["gbw::browser::Browser_engine"]{
        +Browser_engine(Browser_widget: &browser_widget)
        +Browser::apis get_apis()

        #OS impls()
        #gbw::gtk common()
        #virtual::Browser impls()*
        
        -Browser_widget *browser_widget
        
    }

    class Lib_gtk["gbw::gtk::Lib_gtk"]{
      #gbw::gtk common()
      #virtual::OS declarations()*
    }

    namespace OS_libs {
      class Lib_mac["gbw::os::mac::Lib_mac"]{
        #gbw::gtk common()
        #virtual::OS impls()*
      }
      class Lib_win["gbw::os::win::Lib_win"]{
        #gbw::gtk common()
        #virtual::OS impls()*
      }
      class Lib_linux["gbw::os::linuz::Lib_linux"]{
        #gbw::gtk common()
        #virtual::OS impls()*
      }
    }

    namespace Gtk {
      class Widget["Gtk::Widget"]{
        #Gtk::Widget methods()
      }
      class Window["Gtk::Window"] {
        #Gtk::Window methods()
      }
    }


    namespace User_entry {
      class Application["Gtk::Application"]
      class User_application{
        -Browser_widget gbw_widget
      }
    }
  
 namespace Browser {
    class Webkit {
      +api environment_API()
      +api controller_API()
      +api core_API()
    }
    class MsWebview2 {
      +api environment_API()
      +api controller_API()
      +api core_API()
    }
    class Chromium {
      +api environment_API()
      +api controller_API()
      +api core_API()
    }
 }

    Browser_engine <|-- Browser_window
    Browser_engine *-- Webkit : or
    Browser_engine *-- Chromium : or
    Browser_engine *-- MsWebview2 : or
    Browser_engine --* Browser_widget

    Browser_widget --* User_application
    Browser_widget <|-- Widget

    User_application <|-- Application

    Browser_window <|-- Window

    Browser_engine <|.. Browser_widget

    Browser_window <|-- Lib_win : or
    Browser_window <|-- Lib_mac : or
    Browser_window <|-- Lib_linux : or

    Lib_win <|-- Lib_gtk
    Lib_mac <|-- Lib_gtk
    Lib_linux <|-- Lib_gtk
    
```





    
    

    
    

    
    

## `Gtk::Window`, `Gtk::Widget` and embedding
Gtk >= 4 no longer supports embedding native windows within native windows. This leaves two options for embedding a browser engine window:
1) Use Off Screen Rendering (OSR) from the browser engine, render the pixel buffer into a `Gtk::Widget` and map Gtk signals back to the browser.
2) Wrap the native browser window in a new top level `Gtk::Window` and map it's size and position back to a placeholder widget. This approach I am calling pseudo-embedding.

Considering that:
- not all browser engines support OSR, 
- signal mapping can be fragile and difficult to maintain across multiple engines,
- thread-safety is not inherent to GTK but required in some engines,

it has been decided to go with the pseudo-embedding approach for GBW.
