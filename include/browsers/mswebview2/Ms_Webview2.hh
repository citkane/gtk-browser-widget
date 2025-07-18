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

/*****************************************************************************
 * This file contains interfaces and methods for working with the MSWebview2 *
 * embedded browser.                                                         *
 *                                                                           *
 * @see https://developer.microsoft.com/en-us/microsoft-edge/webview2        *
 *                                                                           *
 *****************************************************************************/

#ifndef EBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
#define EBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
#if defined(__WIN32__) && defined(EBW_MSWEBVIEW2)

#include "browsers/mswebview2/lib/Controller.hh"
#include "browsers/mswebview2/lib/Environment.hh"
#include "gtk/Browser_window.hh"
#include "os/windows/Lib_win.hh"
#include "types/types.hh"

using namespace gbw::os::windows;

namespace gbw {
namespace browsers::mswebview2 {

class Ms_Webview2 : Lib_win,
                    lib_mswebview2::Environment,
                    lib_mswebview2::Controller,
                    public gtk::Browser_window {
public:
  ~Ms_Webview2() override;
  Ms_Webview2(ebw_widget_t &widget);

protected:
  /// @copydoc gtk::Browser_window::browser_get_core_imp
  smart_core_t &browser_get_core_impl() override;
  /// @copydoc gtk::Browser_window::browser_get_controller_impl
  smart_control_t &browser_get_controller_impl() override;
  /// @copydoc gtk::Browser_window::set_native_windows_impll
  void set_native_windows_impl(gtk_window_t &browser_window,
                               gtk_window_t &top_level_window) override;
  /// @copydoc gtk::Browser_window::browser_engine_init_impl
  void browser_engine_init_impl() override;
  /// @copydoc gtk::Browser_window::browser_ready_imp
  ready_signal_t &browser_ready_impl() override {
    return browser_ready_signal;
  };

private:
  gtk_widget_t *browser_widget;

  gdk_bounds_t bounds;

  smart_env_t browser_environment;
  smart_control_t browser_control;
  smart_core_t browser_core;

  ready_signal_t browser_environment_ready;
  ready_signal_t browser_ready_signal;

  HWND browser_hWnd;
  HWND top_level_hWnd;
// Dummy trigger for code review 2025-07-18
  void set_browser_env(browser_env_t &environment);
  void set_browser_controller(browser_controller_t &controller);
  void set_browser_core(browser_core_t &core);
  void create_browser_controller_and_core();

  /// Determines UI position and size changes on the @ref gbw::Browser_widget
  /// instance and applies them to the @ref gbw::gtk::Browser_window instance
  /// and it's contained browser display.
  ///
  /// @note Since GTK 4 we can no longer embed a `Gtk::Window` as a child of
  /// another `Gtk::Window`, so we need to implement a pseudo-embed by tracking
  /// UI changes and then natively pass them to pseudo-children.
  ///
  /// For this purpose, We hook into the GTK event loop with `Glib::signal_idle`
// Dummy trigger for code review 2025-07-18
  bool layout_update_cb();
  void fit_browser_to_window(LONG x, LONG y, LONG width, LONG height);

  friend class lib_mswebview2::Environment;
  friend class lib_mswebview2::Controller;
};

} // namespace browsers::mswebview2
using Browser_engine = gbw::browsers::mswebview2::Ms_Webview2;
} // namespace gbw

#endif // __WIN32__
#endif // EBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
