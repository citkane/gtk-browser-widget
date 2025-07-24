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
