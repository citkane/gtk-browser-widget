#ifndef EBW_GTK_BROWSER_WINDOW_HH
#define EBW_GTK_BROWSER_WINDOW_HH

#include "types/types.hh"
#include <gtkmm/window.h>

using namespace gbw::types;

namespace gbw {
class Browser_widget;
namespace gtk {

class Browser_window : public gtk_window_t {
public:
  virtual ~Browser_window() = 0;
  Browser_window(ebw_widget_t &ebw_widget);

  ready_signal_t &ready_signal() { return browser_ready_impl(); }
  smart_core_t &inner_api() { return browser_get_core_impl(); }
  smart_control_t &outer_api() { return browser_get_controller_impl(); }

protected:
  /// Gets the browser core API (ie. the window inner controls)
  virtual smart_core_t &browser_get_core_impl() = 0;

  /// Gets the browser controller API (ie. the window outer controls)
  virtual smart_control_t &browser_get_controller_impl() = 0;

  /// Sets the OS native window handles for the top level and browser window.
  virtual void set_native_windows_impl(gtk_window_t &browser_window,
                                       gtk_window_t &top_level_window) = 0;

  /// Initialises the browser engine and waits for completion signals.
  virtual void browser_engine_init_impl() = 0;

  /// The signal for the browser engine ready event.
  virtual ready_signal_t &browser_ready_impl() = 0;

  gtk_window_t &get_top_level_window() { return *top_level_window; };

private:
  gtk_widget_t *browser_widget;
  gtk_window_t *top_level_window;
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
};
} // namespace gtk

struct browser_api_t {
  smart_control_t &outer;
  smart_core_t &core;
};

} // namespace gbw

#endif // EBW_GTK_BROWSER_WINDOW_HH