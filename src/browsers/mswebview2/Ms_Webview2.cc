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

#include "Browser_widget.hh" // IWYU pragma: keep
#include "browsers/mswebview2/Ms_Webview2.hh"

using namespace gbw::browsers::mswebview2;
using namespace gbw::os::windows;
using namespace gbw::gtk;

Ms_Webview2::~Ms_Webview2() {
  std::cout << "Destroying Browser_window" << std::endl;
  browser_control->Close();
  std::cout << "Destroyed Browser_window" << std::endl;
};

Ms_Webview2::Ms_Webview2(ebw_widget_t &widget)
    : Browser_window(widget), browser_widget(&widget) {};

smart_core_t &Ms_Webview2::browser_get_core_impl() { return browser_core; }

smart_control_t &Ms_Webview2::browser_get_controller_impl() {
  return browser_control;
}
// Dummy trigger for code review 2025-07-18
void Ms_Webview2::set_native_windows_impl(gtk_window_t &browser_window,
                                          gtk_window_t &top_level_window) {

  browser_hWnd = get_hWnd_from_gtk(browser_window);
  top_level_hWnd = get_hWnd_from_gtk(top_level_window);
  bounds = browser_widget->compute_bounds(top_level_window);
};

void Ms_Webview2::browser_engine_init_impl() {
  std::cout << "initialising browser engine\n";
  browser_environment_ready.connect(
      [this] { create_browser_controller_and_core(); });
  auto handler = new Environment::CompletedHandler(*this);
  if (create_webview2(nullptr, nullptr, nullptr, handler) != S_OK) {
    throw ebw_error("Failed to create MsWebview2");
  }
}

void Ms_Webview2::set_browser_env(browser_env_t &environment) {
  std::cout << "- Got MsWebview2 browser environmet\n";
  browser_environment = smart_env_t(&environment);
  browser_environment_ready.emit();
}

void Ms_Webview2::create_browser_controller_and_core() {
  auto handler = new Controller::CompletedHandler(*this);
  if (browser_environment->CreateCoreWebView2Controller(browser_hWnd,
                                                        handler) != S_OK) {
    throw ebw_error("Failed to CreateCoreWebView2Controller");
  }
}

void Ms_Webview2::set_browser_controller(browser_controller_t &controller) {
  std::cout << "- Got MsWebview2 browser contoller\n";
  browser_control = smart_control_t(&controller);
  fit_browser_to_window(0, 0, bounds->get_width(), bounds->get_height());
}

void Ms_Webview2::set_browser_core(browser_core_t &core) {
  std::cout << "- Got MsWebview2 browser core\n";
  browser_core = smart_core_t(&core);
  browser_control->put_IsVisible(TRUE);

  Glib::signal_idle().connect([this] { return layout_update_cb(); });

  browser_ready_signal.emit();
}
// Dummy trigger for code review 2025-07-18
bool Ms_Webview2::layout_update_cb() {
  auto &top_level_window = get_top_level_window();
  auto fudge = get_csd_fudge();

  // Get the absolute screen position of the browser top level HWND
  POINT topLeft = {0, 0};
  ClientToScreen(top_level_hWnd, &topLeft);

  // Get the size and relative position of the browser widget
  auto new_bounds = browser_widget->compute_bounds(top_level_window);
  auto origin = new_bounds->get_origin();

  // For CSD managed Gtk::Windows, we need to positionally compensate for the
  // GTK decorations which are included in the HWND measurement.
  auto decoration_offset =
      get_decorations_offset(top_level_hWnd, top_level_window, fudge);

  // Transform the relative position to include the absolute screen position and
  // GTK window decoration offset
  origin.set_x(topLeft.x + origin.get_x() + decoration_offset.x);
  origin.set_y(topLeft.y + origin.get_y() + decoration_offset.y);
  new_bounds->set_origin(origin);

  // No-op if layout has not changed.
  if (bounds->equal(*new_bounds)) {
    return true;
  }

  auto const width = new_bounds->get_width();
  auto const height = new_bounds->get_height();
  auto const X = new_bounds->get_x();
  auto const Y = new_bounds->get_y();

  // Use native MS Windows control to move the browser HWND to location.
  MoveWindow(browser_hWnd, X, Y, width, height, FALSE);
  // Use native GTK control to resize the browser Gtk::Window (GTK locks
  // internal size management)
  set_default_size(width, height);

  // Check if the window is just moving.
  // Avoid calling re-size on the browser uneccesarilly
  if (bounds->get_height() == height && bounds->get_width() == width) {
    return true;
  }
  fit_browser_to_window(0, 0, width, height);
  //   Update the bounds class instance.
  bounds->swap(*new_bounds);
  return true;
}

void Ms_Webview2::fit_browser_to_window(LONG x, LONG y, LONG width,
                                        LONG height) {
  // Re-size the browser with the MsWebview2 controller API.
  RECT const rect{x, y, width + x, height + x};
  browser_control->put_Bounds(rect);
};
