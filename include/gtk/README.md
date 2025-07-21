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
classDiagram
    Browser_widget *-- User_application
    User_application <|-- Application

    Browser_engine ()..() Webkit : or
    Browser_engine ()..() Chromium : or
    Browser_engine ()..() MsWebview2 : or
    Browser_engine --* Browser_widget
    Browser_engine *.. Browser_widget : *widget

    Webkit <|-- Browser_window
    Chromium <|-- Browser_window
    MsWebview2 <|-- Browser_window
    
    Browser_widget <|-- Widget
    Browser_window <|-- Window
    
    Browser_window <|-- Lib_gtk
    Lib_gtk <|-- Lib_win : or
    Lib_gtk <|-- Lib_mac : or
    Lib_gtk <|-- Lib_linux : or
    
    Lib_win <|-- Lib_os
    Lib_mac <|-- Lib_os
    Lib_linux <|-- Lib_os
    Lib_os <|-- Lib_browser
    
    namespace User_entry {
      class Application["Gtk::Application"]:::other
      class User_application:::other {
        -Browser_widget gbw_widget
      }
    }
    
    namespace virtual_declarations {
      class Lib_browser["gbw::browsers::Lib_browser"]:::gbw {
        <<Abstract>> 
        #virtual browser_declarations()*
        #OS impls()
        #gbw::gtk common()
      } 
      
      class Lib_os["gbw::os::Lib_os"]:::gbw {
        <<Abstract>>
        #virtual OS_declarations()*
        #gbw::gtk common()
      }
    }

    namespace gbw_custom_Gtk {
      class Browser_widget["gbw::Browser_widget"]:::gbw {
          +sigc::signal ready()
          +Browser_core::apis get_browser_apis()
          #Gtk::Widget.methods()
          -Browser_engine browser_engine
      }

      class Browser_window["gbw::gtk::Browser_window"]:::gbw {
        #Gtk::Window.methods()
        #gbw::gtk common()
        #OS methods()
        #virtual Browser_declarations()*
      }
    }

    class Lib_gtk["gbw::gtk::Lib_gtk"]:::gbw {
      -gbw::Browser_widget widget
      -gbw::Browser_window window
      #gbw::gtk common()
    }

    namespace OS {
      class Lib_mac["gbw::os::mac::Lib_mac"]:::gbw {
        #gbw::gtk common()
        #virtual OS_impls()*
      }
      class Lib_win["gbw::os::win::Lib_win"]:::gbw {
        #gbw::gtk common()
        #virtual OS_impls()*
      }
      class Lib_linux["gbw::os::linuz::Lib_linux"]:::gbw {
        #gbw::gtk common()
        #virtual OS_impls()*
      }
    }

    namespace Gtk {
      class Widget["Gtk::Widget"]:::other {
        #Gtk::Widget.methods()
      }
      class Window["Gtk::Window"]:::other {
        #Gtk::Window.methods()
      }
    }


    class Browser_engine["gbw::browser::Browser_engine (aliased)"]:::other {
        +Browser_engine(Browser_widget: &browser_widget)
        +Browser::apis apis

        #OS impls()
        #virtual browser_impls()*
        
        -Browser::apis init_browser()
        
    }

    namespace gbw_Browser_classes {
          class Webkit["gbw::browsers::webkit"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
        }
        class MsWebview2["gbw::browsers::mswebview2"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
        }
        class Chromium["gbw::browsers::chromium"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
        }
    }

    classDef gbw fill:#FAE8FF; 
    classDef other fill:#FFFFFF00
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
