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

#ifndef EBW_GTK_BROWSER_WINDOW_HH
#define EBW_GTK_BROWSER_WINDOW_HH

#include "os/includes.hh" // IWYU pragma: keep
#include "types/types.hh"

namespace gbw {
class Browser_widget;
namespace gtk {

class Browser_window : public gtk_window_t, protected Lib_os {
public:
  // virtual ~Browser_window() = 0;
  Browser_window(gbw_widget_t *widget);

  ready_signal_t &ready_signal() { return browser_ready_impl(); }
  smart_core_t &inner_api() { return browser_get_core_impl(); }
  smart_control_t &outer_api() { return browser_get_controller_impl(); }

protected:
  /// Gets the browser core API (ie. the window inner controls)
  virtual smart_core_t &browser_get_core_impl() = 0;

  /// Gets the browser controller API (ie. the window outer controls)
  virtual smart_control_t &browser_get_controller_impl() = 0;

  /// Initialises the browser engine and waits for completion signals.
  virtual void browser_engine_init_impl() = 0;

  /// The signal for the browser engine ready event.
  virtual ready_signal_t &browser_ready_impl() = 0;

private:
  bool focus_initialised;

  /// Callback for `gtk_window_t.signal_realize`
  void window_realised_cb();
  /// Callback for when the browser window is realised and present on screen
  ///
  /// @note This is triggered by `Glib::signal_idle().connect_once` nested
  /// within `gtk_window_t.signal_realize` so that we correctly catch the
  /// application bootstrap sequence
  void window_ready_cb();

  /// Sets the initial GTK properties for the @ref gbw::gtk::Browser_window
  void browser_window_init_config();

  /// Sets a controller to maintain visual focus on the app window instead of
  /// the native browser window when the input switches.
  void set_focus_controller();

  friend class gbw::gtk::Lib_gtk;
};
} // namespace gtk

struct browser_api_t {
  smart_control_t &outer;
  smart_core_t &core;
};

} // namespace gbw

#endif // EBW_GTK_BROWSER_WINDOW_HH
