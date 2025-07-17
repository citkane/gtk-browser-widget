[Home](../../README.md)<br>
[Development notes](../README.md)

# Gtk 4

## `Gtk::Window`, `Gtk::Widget` and embedding
Gtk >= 4 no longer supports embedding native windows within native windows. This leaves 2 options for embedding a browser engine window:
- Use Off Screen Rendering (OSR) from the browser engine, render the pixel buffer into a Gtk::Widget and map Gtk signals back to the browser.
- Create the browser window in a new top level Gtk::Window and track it's size and position back to a placeholder widget. This approach I am calling pseudo-embedding.

Seeing that:
- not all browser engines support OSR, 
- signal mapping can be fragile as browser engine versions increment
- thread-safety is not inherent to GTK

It has been decided to go with the pseudo-embedding approach for GBW.