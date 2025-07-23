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

#ifndef GBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
#define GBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
#if defined(_WIN32) && defined(GBW_MSWEBVIEW2)

#include "browsers/mswebview2/lib/Controller.hh"
#include "browsers/mswebview2/lib/Environment.hh"
#include "core/lib/Lib_gbw.hh"
#include "types/types.hh"

using namespace gbw::core::lib;
namespace gbw::browsers::mswebview2 {

class Ms_Webview2 : lib_mswebview2::Environment,
                    lib_mswebview2::Controller,
                    protected Lib_gbw {

  struct mswebview_api_layout_t : browser_api_layout_t {
    mswebview_api_layout_t(Ms_Webview2 *self) : browser_api_layout_t(self) {};

    void fit(layout_t &layout) override;
  };

  struct mswebview_api_signals_t : browser_api_signals_t {
    mswebview_api_signals_t(Ms_Webview2 *self) : browser_api_signals_t(self) {}

    ready_signal_t &core_ready() override;
    ready_signal_t &env_ready() override;
    ready_signal_t &controller_ready() override;
  };

  struct mswebview_api_api_t : browser_api_api_t {
    mswebview_api_api_t(Ms_Webview2 *self) : browser_api_api_t(self) {};

    smart_core_t &core() override;

    smart_control_t &controller() override;

    smart_env_t &environment() override;
  };

  struct mswebview_api_t : browser_api_t {
    mswebview_api_t(Ms_Webview2 *self)
        : browser_api_t(self, &self->mswebview_api_api,
                        &self->mswebview_api_layout,
                        &self->mswebview_api_signals) {};

    void init() override;
  };

public:
  virtual ~Ms_Webview2() = 0;
  Ms_Webview2(gbw_widget_t *widget, gbw_browser_t *engine);

protected:
  mswebview_api_t browser;

private:
  mswebview_api_api_t mswebview_api_api;
  mswebview_api_layout_t mswebview_api_layout;
  mswebview_api_signals_t mswebview_api_signals;

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
  // bool layout_update_cb();

  // void fit_browser_to_window(LONG x, LONG y, LONG width, LONG height);

  friend class lib_mswebview2::Environment;
  friend class lib_mswebview2::Controller;
};

} // namespace gbw::browsers::mswebview2

#endif // _WIN32
#endif // GBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
