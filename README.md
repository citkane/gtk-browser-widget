<h1 align="center"> Gtk Browser Widget (GBW) </h1>

<p align="center">
  <img width="600" alt="Gtk Browser Widget" src="https://github.com/user-attachments/assets/6933e9b3-8a31-47a9-9b77-c05b70e1e420" />
</p>


GBW is a cross platform [GTK 4](https://www.gtk.org/) widget that makes available an embedded browser engine of your choice:
- Webkit (TODO)
- Chromium (TODO)
- MsWebview2 (ALPHA stage)

It is developed with the [gtkmm 4](https://gtkmm.gnome.org/en/) C++  library.<br>
More language bindings are planned for the future.

### Basic usage:
```c++
// gtkmm headers MUST be included first
#include <gtkmm/applicationwindow.h>

#include "Browser_widget.hh"

class Application : public Gtk::ApplicationWindow {
public:
  Application() : browser() {

    browser.ready().connect([this] {
      auto gbw = browser.api().core;
      gbw->Navigate(L"https://www.gtk.org/");
    });

    set_title("Example GBW application");
    set_default_size(1000, 900);
    set_child(browser);
  };
private:
  gbw::Browser_widget browser;
};


#ifdef __WIN32__
int CALLBACK WinMain(_In_ HINSTANCE /*hInstance*/,
                     _In_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/,
                     _In_ int /*nCmdShow*/) {
#else
int main() {
#endif
  auto app = Gtk::Application::create("org.gbw.example");
  return app->make_window_and_run<Application>(0, nullptr);
};
```

### Installation
At this ALPHA stage, only MsWebview2 for Windows browser engine option is available.<br>
You will need [MSys2](https://www.msys2.org/) installed, and then proceed from a MSys2 UCRT bash terminal:
```bash
# Install Msys2 system dependencies
pacman -Suy # Do this twice for a fresh install to update the core
pacman -S mingw-w64-ucrt-x86_64-clang-tools-extra
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-gtkmm-4.0

# Clone the GBW repository
git clone https://github.com/citkane/gtk-browser-widget.git
cd gtk-browser-widget

# Use the installer script
source installer.sh
packages_install
set_target examples/mswebview2/main.cc
generate
build
run

```
## Example code
- [MsWebview2](examples/mswebview2)
- Webkit
- Chromium
## Development
[Development notes](include/README.md)

GBW wants to uncomplicate and unboilerplate the app development experience. What it does:
- Starts a browser engine instance
- Presents a browser window in a Gtk:Window
- [Pseudo-embeds](include/gtk/README.md#gtkwindow-gtkwidget-and-embedding) the window in the user's Gtk::Widget
- Presents to the developer a full API of their chosen web engine

From there, it is the developer's prerogative to plumb up their GTK application to the browser API in a way that suits their logic. GBW is not a framework, but rather a simple tool.
