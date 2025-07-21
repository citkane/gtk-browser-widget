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
---
config:
  theme: 'base'
  class:
    hideEmptyMembersBox: true
---
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
      } 
      
      class Lib_os["gbw::os::Lib_os"]:::gbw {
        <<Abstract>>
        #virtual_OS declarations()*
      }
    }

    namespace gbw_custom_Gtk {
      class Browser_widget["gbw::Browser_widget"]:::gbw {
          +sigc::signal ready()
          +Browser_core::apis get_browser_apis()
          #Gtk::Widget methods()
          -Browser_engine *browser_engine
      }

      class Browser_window["gbw::gtk::Browser_window"]:::gbw {
        +Browser_window(Browser_widget *widget)
        #Gtk::Window methods()
        #gbw::gtk methods()
        #OS methods()
        #Browser methods()
      }
    }

    class Lib_gtk["gbw::gtk::Lib_gtk"]:::gbw {
      +Lib_gtk(Browser_widget *widget, Browser_window *window)
      -gbw::Browser_widget *widget
      -gbw::Browser_window *window
      #gbw::gtk methods()
      #OS methods()
      #Browser methods()
    }

    namespace OS {
      class Lib_mac["gbw::os::mac::Lib_mac"]:::gbw {
        #gbw::gtk methods()
        #virtual_OS impls()*
      }
      class Lib_win["gbw::os::win::Lib_win"]:::gbw {
        #gbw::gtk methods()
        #virtual_OS impls()*
      }
      class Lib_linux["gbw::os::linuz::Lib_linux"]:::gbw {
        #gbw::gtk methods()
        #Browser methods()
        #virtual_OS impls()*
      }
    }

    namespace Gtk {
      class Widget["Gtk::Widget"]:::other {
        #Gtk::Widget methods()
      }
      class Window["Gtk::Window"]:::other {
        #Gtk::Window methods()
      }
    }


    note for Browser_engine "Pragma included from 'gtk/Browser_engine.hh'<br>Each 'browsers/*/*.hh' header aliases it's Browser class to 'gbw::browsers::Browser_engine'" 
    class Browser_engine["gbw::browser::Browser_engine"]:::other {
        <<Aliased>>
        +Browser_engine(Browser_widget: *browser_widget)
        +Browser::apis apis
        -Browser::apis init_browser()
        
    }

    namespace gbw_Browser_classes {
          class Webkit["gbw::browsers::webkit"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
          #gbw::gtk methods()
          #OS methods()
          #virtual_Browser impls()*
        }
        class MsWebview2["gbw::browsers::mswebview2"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
          #gbw::gtk methods()
          #OS methods()          
          #virtual_Browser impls()*
        }
        class Chromium["gbw::browsers::chromium"]:::gbw {
          +api environment_API()
          +api controller_API()
          +api core_API()
          #gbw::gtk methods()
          #OS methods()          
          #virtual_Browser impls()*
        }
    }

    classDef gbw fill:#FCF5FF, stroke:#343238 
    classDef other fill:#FFFFFF00, stroke:#343238
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
