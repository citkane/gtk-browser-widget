[Home](../#development)
# Development Notes
GBW is developed for GTK 4 using the official C++ interface, [gtkmm 4](https://gtkmm.gnome.org/en/).
- [Getting Started with gtkmm](https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/index.html)
- [API reference](https://gnome.pages.gitlab.gnome.org/gtkmm/index.html)

## Gbw_widget.hh
This is the consumer-facing entry class that provides GBW. It is a [custom `Gtk::Widget`](https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/chapter-customwidgets.html)

## [Gtk 4](core)
Development should aim to offload operations to the gtkmm API. Guiding principles:
- Do not get caught up in low level system complexities. Chances are that the GTK developers have already resolved it somewhere in the API.
- Keep GBW/GTK related logic in the `core` folder/domain. When logic becomes os/browser specific, use pure virtual methods in the `Lib_<os|browser>.hh` headers with a naming logic that will suit all contexts.
- GTK 4 has some limitations when compared to earlier releases. Most significantly for the GBW context is that windows can no longer be embedded into another window. This is expanded on further in the chapter header link.

## [Operating Systems](os)
GBW will support:
- Windows (10+ at minimum, but probably all)
- Linux (X11 and Wayland compositors)
- Mac (Cocoa)

OS specific classes MUST provide implementation to higher level pure virtual methods, and it is important that naming conventions for these are respected across all domains.

## [Browser Engines](browsers)
GBW will support:
- [MsWebview2](https://developer.microsoft.com/en-us/microsoft-edge/webview2) (Windows only).
- Chromium (Windows, Linux, Mac) - backed by [Chromium Embedded Framework](https://github.com/chromiumembedded/cef).
- Webkit (Windows, Linux, Mac) - backed by [[WPE](https://webkit.org/wpe/) or [Webkit](https://webkit.org/project/) or [WebkitGtk](https://webkitgtk.org/)]. See [this discussion](https://github.com/citkane/gtk-browser-widget/discussions/4).
