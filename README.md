<h1 align="center"> Gtk Browser Widget (GBW) </h1>

<p align="center">
  <img width="600" alt="Gtk Browser Widget" src="https://github.com/user-attachments/assets/6933e9b3-8a31-47a9-9b77-c05b70e1e420" />
</p>


GBW is a cross-platform [GTK 4](https://www.gtk.org/) widget that makes available an embedded browser engine of your choice:
- MsWebview2 (ALPHA)
- Webkit (todo) - [discussion](https://github.com/citkane/gtk-browser-widget/discussions/4)
- Chromium (todo)

GBW is developed with the [gtkmm 4](https://gtkmm.gnome.org/en/) C++  library. More [language bindings](#language-bindings-todo) are planned for the future.

### Minimal usage:
```c++
// gtkmm headers MUST be included first
#include <gtkmm/applicationwindow.h>

#include "Gbw_widget.hh"

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
  gbw::Gbw_widget browser;
};


#ifdef _WIN32
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
set_browser mswebview2 # default is Chromium
packages_install
set_target examples/mswebview2/main.cc
generate
build
install
run

```
## Example code
- [MsWebview2](examples/mswebview2)
- Webkit (todo)
- Chromium (todo)

## Contributing
Please [fork](https://github.com/citkane/gtk-browser-widget/fork) this repository and submit pull requests against the [development branch](https://github.com/citkane/gtk-browser-widget/tree/development).

## Development
Development notes:
- [Gtk 4](include#gtk-4)
- [Operating Systems](include#operating-systems)
- [Browser Engines](include#browser-engines)

GBW wants to uncomplicate and unboilerplate the embedded browser app development experience. What it does:
- Starts a browser engine instance,
- presents a browser window in a new top level `Gtk:Window`
- [pseudo-embeds](include/core/#gtkwindow-gtkwidget-and-embedding) the window into the `gbw::Gbw_widget` instance,
- provides to the developer a full API of their chosen web engine

From there, it is the developer's prerogative to plumb up their GTK application to the browser API in a way that suits their logic. GBW is not a framework, but rather a simple tool.

## Plugins
Plugins optionally extend GBW to provide functionallity for common usage patterns.

#### Bind (todo)
Provides an easy to consume interface that binds native callback functions to browser-side javascript functions.

## Language Bindings (todo)
GBW is developed in C++, but will aim to provide bindings for other GTK supported languages.

Support for the following languages is planned:


- C
- JavaScript
- Perl
- Python
- Rust
- Vala 
- C# (via Gir.Core)
