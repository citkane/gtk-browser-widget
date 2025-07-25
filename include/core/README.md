[Home](../../#development)<br>
[Development notes](..)

# Gtk
This folder is for integrations and presentation of the browser engine window into a `Gtk::Window`

## Gbw_window.hh
Provides the top level `Gtk::Window` instance that will wrap the native browser engine window.

## Browser.hh
Header file that uses pragma guards to include the relevant browser engine class header. The included browser header file MUST alias it's class to `gbw::browser::Browser`

## Class Hierarchy
```mermaid
---
config:
  theme: 'base'
  class:
    hideEmptyMembersBox: true
---
classDiagram
    Gbw_widget --* User_application
    User_application <|-- Application

    Gbw_widget <|-- Widget
    Gbw_window <|-- Window
    
    Gbw_widget *--* Gbw_window : <- *engine<br>*widget ->

    Webkit <|-- Gbw_base
    Chromium <|-- Gbw_base
    MsWebview2 <|-- Gbw_base

    Gbw_window <|-- Webkit  : or
    Gbw_window <|-- Chromium : or
    Gbw_window <|-- MsWebview2 : or

    Gbw_base <|-- Lib_win : or
    Gbw_base <|-- Lib_mac : or
    Gbw_base <|-- Lib_linux : or
    
    Lib_win <|-- Os_base
    Lib_mac <|-- Os_base
    Lib_linux <|-- Os_base
    Os_base <|-- Browser_base
    
    namespace Gtk {
      class Widget["Gtk::Widget"]:::other {
        #Gtk::Widget methods()
      }
      class Window["Gtk::Window"]:::other {
        #Gtk::Window methods()
      }
    }

    namespace User_entry {
      class Application["Gtk::Application"]:::other
      class User_application:::other {
        -Gbw_widget gbw_widget
      }
    }

    namespace Custom_Gtk {
      class Gbw_widget["gbw::Gbw_widget"]:::gbw {
          +Browser::api_t browser

          -Gbw_Window *engine
          -Os::api_t engine->os
          -Gbw_base::api_t engine->gbw

          #Browser::api_t get_browser_api()
          -Gtk::Widget methods()
      }

      class Gbw_window["gbw::core::Gbw_window"]:::gbw {
          +Gbw_window(Gbw_widget *widget)

          #Browser::api_t browser
          #Os::api_t os
          #Gbw_base::api_t gbw

          #Gbw_base accessors()           
          #Os accessors()        
          #Browser accessors()
          
          -Gtk::Window methods()
      }
    }

    namespace gbw_browser_Browser {
          class Webkit["gbw::browsers::Webkit"]:::gbw {
          +Webkit(Gbw_widget *widget, Gbw_window *window)

          #Browser::api_t browser
          #Os::api_t os
          #Gbw_base::api_t gbw

          #Gbw_base accessors()           
          #Os accessors()
          #Browser accessors()
          #Browser::api_t virtual_implementations()*
        }

        class MsWebview2["gbw::browsers::MsWebview2"]:::gbw {
          +MsWebview2(Gbw_widget *widget, Gbw_window *window)

          #Browser::api_t browser
          #Os::api_t os
          #Gbw_base::api_t gbw

          #Gbw_base accessors()           
          #Os accessors()
        
          #Browser accessors()
          #Browser::api_t virtual_implementations()**
        }

        class Chromium["gbw::browsers::Chromium"]:::gbw {
          +Chromium(Gbw_widget *widget, Gbw_window *window)

          #Browser::api_t browser
          #Os::api_t os
          #Gbw_base::api_t gbw

          #Gbw_base accessors()          
          #Os accessors()
        
          #Browser accessors()
          #Browser::api_t virtual_implementations()*
        }
    }

    class Gbw_base["gbw::core::Gbw_base"]:::gbw {
        +Gbw_base(Gbw_widget *widget, Gbw_window *window)
        
        #Os::api_t os
        #Gbw_base::api_t gbw
        
        -Gbw_widget *widget
        -Gbw_window *window
        -Gbw_base vars

        #Gbw_base accessors()
        #Gbw_base::api_t methods()
        
        #Os accessors()

        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
    }

    namespace gbw_os_Os {
      class Lib_mac["gbw::os::Lib_mac"]:::gbw {
        <<API os.*>>
        #Os::api_t os
        
        #Os accessors()
        #Os::api_t virtual_implementations()*
        
        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
      }
      class Lib_win["gbw::os::Lib_win"]:::gbw {
        <<API os.*>>
        #Os::api_t os
        
        #Os accessors()
        #Os::api_t virtual_implementations()*
        
        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
      }
      class Lib_linux["gbw::os::Lib_linux"]:::gbw {
        <<API os.*>>
        #Os::api_t os
        
        #Os accessors()
        #Os::api_t virtual_implementations()*
        
        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
      }
    }

    namespace Base_classes {
 
      class Os_base["gbw::os::Os_base"]:::gbw {
        <<Base>>
        -Os vars
        
        #Os accessors()
        #Os::api_t common_methods() 
        #Os::api_t virtual_declarations()*
        
        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
      }

      class Browser_base["gbw::browsers::Browser_base"]:::gbw {
        <<Base>> 
        -Browser vars
        
        #Browser accessors()
        #Browser::api_t common_methods()
        #Browser::api_t virtual_declarations()*
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
