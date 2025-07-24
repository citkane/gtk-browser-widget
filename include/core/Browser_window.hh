/*
 * MIT License
 *
 * Copyright (c) 2025 Michael Jonker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GBW_CORE_BROWSER_WINDOW_HH
#define GBW_CORE_BROWSER_WINDOW_HH

#include "core/Browser_engine.hh" // IWYU pragma: keep
#include "types/types.hh"

namespace gbw {

class Browser_widget;

namespace core {

class Browser_window : public gtk_window_t, public Browser_engine {
public:
  ~Browser_window() override;
  Browser_window(gbw_widget_t *widget);

private:
  bool focus_initialised;
  layout_t current_layout{0, 0, 0, 0};

  /// Callback for `gtk_window_t.signal_realize`
  void window_realised_cb();
  /// Callback for when the browser window is realised and present on screen
  ///
  /// @note This is triggered by `Glib::signal_idle().connect_once` nested
  /// within `gtk_window_t.signal_realize`. The `Gtk::Window`s  are now realised
  /// AND shown, so we correctly catch the application bootstrap sequence
  void window_ready_cb();

  /// Sets the initial GTK properties for the @ref Browser_window
  void browser_window_init_config();

  /// Sets a controller to maintain visual focus on the app window instead of
  /// the native browser window when the input switches.
  void set_focus_controller();

  /// Determines UI position and size changes on the @ref gbw::Browser_widget
  /// instance and applies them to the @ref gbw::gtk::Browser_window instance
  /// and it's contained browser display.
  ///
  /// @note Since GTK 4 we can no longer embed a `Gtk::Window` as a child of
  /// another `Gtk::Window`, so we need to implement a pseudo-embed by tracking
  /// UI changes and then natively pass them to pseudo-children.
  ///
  /// For this purpose, We hook into the GTK event loop with `Glib::signal_idle`
  void layout_update(layout_t &new_layout);

  friend class gbw::Browser_widget;
};

} // namespace core

} // namespace gbw

#endif // GBW_CORE_BROWSER_WINDOW_HH
