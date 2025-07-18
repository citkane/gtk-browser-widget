[Home](..)
# Development Notes
## Contributing
Please [fork](https://github.com/citkane/gtk-browser-widget/fork) this repository and then submit pull requests against the [development branch](https://github.com/citkane/gtk-browser-widget/tree/development).

## [Gtk 4](gtk)
All development should aim to offload operations to the GTK 4 API. Guiding principles:
- Do not get caught up in low level system complexities. Chances are that the GTK developers have already resolved it somewhere in the API.
- Keep GTK related code in the gtk folder/domain. When this forks into OS/browser specific logic, use pure virtual methods with a naming logic that will suit all contexts.
- GTK 4 has some limitions when compared to earlier releases. Most significantly for the GBW context is that windows can no longer be embedded into another window. This is expanded on further in the GTK readme.

## [Operating Systems](os)
GBW will support:
- Windows (10+ at minimum, but probably all)
- Linux (X11 and Wayland compositors)
- Mac (Cocoa)

All OS specific code should provide implementation to higher level pure virtual methods, and it is important that naming conventions for these are respected across all domains.

## [Browser Engines](browsers)
GBW will support:
- [MsWebview2](https://developer.microsoft.com/en-us/microsoft-edge/webview2) (Windows only).
- Chromium (Windows, Linux, Mac) - backed by [Chromium Embedded Framework](https://github.com/chromiumembedded/cef).
- Webkit (Windows, Linux, Mac) - backed by [[WPE](https://webkit.org/wpe/) or [Webkit](https://webkit.org/project/) or [WebkitGtk](https://webkitgtk.org/)]. See [this discussion](https://github.com/citkane/gtk-browser-widget/discussions/4).
