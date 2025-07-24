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

#include <glibmm.h>

#include "browsers/mswebview2/Ms_Webview2.hh"

using namespace gbw;

Ms_Webview2::~Ms_Webview2() {}

Ms_Webview2::Ms_Webview2(gbw_widget_t *widget, gbw_browser_t *engine)
    : Lib_gbw(widget, engine), browser(this), layout_api(this) {};

Ms_Webview2::mswebview_api_layout_t::mswebview_api_layout_t(Ms_Webview2 *self)
    : browser_api_layout_t(self) {};

void Ms_Webview2::mswebview_api_layout_t::fit(layout_t &layout) {
  RECT const rect{0, 0, layout.width, layout.height};
  self->browser.api.controller()->put_Bounds(rect);
};

Ms_Webview2::mswebview_api_t::mswebview_api_t(Ms_Webview2 *self)
    : browser_api_t(self) {};

void Ms_Webview2::mswebview_api_t::init() {
  std::cout << "Initialising MsWebview2 engine\n";
  self->browser.signals.env_ready().connect(
      [this] { self->create_browser_controller_and_core(); });

  auto handler = new Environment::CompletedHandler(*self);

  if (create_webview2(nullptr, nullptr, nullptr, handler) != S_OK) {
    throw gbw_error("Failed to create MsWebview2");
  }
};

void Ms_Webview2::create_browser_controller_and_core() {
  if (!gbw.windows_are_ready()) {
    gbw.signals.windows_are_ready().connect(
        [this] { create_browser_controller_and_core(); });
    return;
  }

  auto handler = new Controller::CompletedHandler(*this);
  auto res = browser.api.environment()->CreateCoreWebView2Controller(
      os.window.browser(), handler);
  if (res != S_OK) {
    throw gbw_error("Failed to CreateCoreWebView2Controller");
  }
}

// void Ms_Webview2::set_browser_env(browser_env_t &environment) {
//   std::cout << "- Got MsWebview2 browser environment\n";
//   set_api_environment(environment);
//   browser.signals.env_ready().emit();
// }
//
// void Ms_Webview2::set_browser_controller(browser_controller_t &controller) {
//   std::cout << "- Got MsWebview2 browser controller\n";
//   api_controller = smart_control_t(&controller);
//   browser.signals.controller_ready().emit();
//   /// fit_browser_to_window(0, 0, bounds->get_width(), bounds->get_height());
// }
//
// void Ms_Webview2::set_browser_core(browser_core_t &core) {
//   std::cout << "- Got MsWebview2 browser core\n";
//   api_core = smart_core_t(&core);
//   browser.signals.core_ready().emit();
//   /// api_controller->put_IsVisible(TRUE);
//
//   // Glib::signal_idle().connect([this] { return layout_update_cb(); });
//
//   /// browser.core_ready_signal().emit();
// }

// ready_signal_t &Ms_Webview2::mswebview_api_signals_t::core_ready() {
//   return self->get_core_ready_signal();
// };
// ready_signal_t &Ms_Webview2::mswebview_api_signals_t::env_ready() {
//   return self->get_environment_ready_signal();
// };
// ready_signal_t &Ms_Webview2::mswebview_api_signals_t::controller_ready() {
//   return self->get_controller_ready_signal();
// };

// smart_core_t &Ms_Webview2::mswebview_api_api_t::core() {
//   return self->get_api_core();
// };
//
// smart_control_t &Ms_Webview2::mswebview_api_api_t::controller() {
//   return self->get_api_controller();
// };
//
// smart_env_t &Ms_Webview2::mswebview_api_api_t::environment() {
//   return self->get_api_environment();
// };

// smart_core_t &Ms_Webview2::browser_get_core_impl() { return browser_core; }
//
// smart_control_t &Ms_Webview2::browser_get_controller_impl() {
//   return browser_control;
// }
//
//// void Ms_Webview2::set_native_windows_impl(gtk_window_t &browser_window,
////                                           gtk_window_t &top_level_window) {
////
////   browser_hWnd = get_native_window_from_gtk(browser_window);
////   top_level_hWnd = get_native_window_from_gtk(top_level_window);
////   bounds = browser_widget->compute_bounds(top_level_window);
//// };
//
// void Ms_Webview2::browser_engine_init_impl() {
//  std::cout << "initialising browser engine\n";
//  browser_environment_ready.connect(
//      [this] { create_browser_controller_and_core(); });
//  auto handler = new Environment::CompletedHandler(*this);
//  if (create_webview2(nullptr, nullptr, nullptr, handler) != S_OK) {
//    throw gbw_error("Failed to create MsWebview2");
//  }
//}

// bool Ms_Webview2::layout_update_cb() {
//   auto native_top_level_window = os.get_top_level_window();
//   auto fudge = gbw.csd.fudge();
//   auto native_size = os.window.get_size(native_top_level_window);
//   auto native_position = os.window.get_position(native_top_level_window);
//
//   // Get the size and relative position of the browser widget
//   auto new_bounds = compute_bounds(native_top_level_window);
//   auto origin = new_bounds->get_origin();
//
//   // For CSD managed Gtk::Windows, we need to positionally compensate for the
//   // GTK decorations which are included in the HWND measurement.
//   auto decoration_offset =
//       gbw.csd
//           . // csd_decorations_offset_imp(top_level_hWnd, top_level_window,
//             // fudge);
//
//       // Transform the relative position to include the absolute screen
//       position
//       // and GTK window decoration offset
//       origin.set_x(topLeft.x + origin.get_x() + decoration_offset.x);
//   origin.set_y(topLeft.y + origin.get_y() + decoration_offset.y);
//   new_bounds->set_origin(origin);
//
//   // No-op if layout has not changed.
//   if (bounds->equal(*new_bounds)) {
//     return true;
//   }
//
//   auto const width = new_bounds->get_width();
//   auto const height = new_bounds->get_height();
//   auto const X = new_bounds->get_x();
//   auto const Y = new_bounds->get_y();
//
//   // Use native MS Windows control to move the browser HWND to location.
//   MoveWindow(browser_hWnd, X, Y, width, height, FALSE);
//   // Use native GTK control to resize the browser Gtk::Window (GTK locks
//   // internal size management)
//   set_default_size(width, height);
//
//   // Check if the window is just moving.
//   // Avoid calling re-size on the browser unnecessarily
//   if (bounds->get_height() == height && bounds->get_width() == width) {
//     return true;
//   }
//   fit_browser_to_window(0, 0, width, height);
//   //   Update the bounds class instance.
//   bounds->swap(*new_bounds);
//   return true;
// }
