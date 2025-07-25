/*
 * MIT License
 *
 * Copyright (c) 2025 Zachary Kleiman contributing to Michael Jonker's project
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

#include "browsers/chromium/Chromium.hh"

// Below libraries necessary for CEF to integrate. Handled by Copilot
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"

using namespace gbw;

Chromium::~Chromium() {}

Chromium::Chromium(gbw_widget_t *widget, gbw_browser_t *engine)
    : Gbw_base(widget, engine), browser(this), layout_api(this) {};

Chromium::api_layout_t::api_layout_t(Chromium *self)
    : Browser_base::api_layout_t(self) {};

void Chromium::api_layout_t::fit(layout_t &layout) {
  if (self->browser.api.controller()) {
    auto hwnd = self->os.window.browser();
    CefWindowHandle window_handle = reinterpret_cast<CefWindowHandle>(hwnd);
    CefRect rect(0,0, layout.width, layout.height);
    self->browser.api.controller()->GetHost()->SetBounds(rect);
  }
};

Chromium::api_t::api_t(Chromium *self) : Browser_base::api_t(self) {};

void Chromium::api_t::init() {
  std::cout << "Initialising Chromium (CEF) engine\n";
  self->browser.signals.env_ready().connect(
      [this] { self->create_browser_controller_and_core(); });

  CefMainArgs main_args;
  CefSettings settings;
  settings.no_sandbox // REVIEW THIS. THIS IS TEMPORARY. JONKER PLEASE DO A REVIEW ABOUT WHAT TO DO WITH THE SANDBOX

  if (!CefInitialize(main_args, settings, nullptr, nullptr)) {
    throw gbw_error("Failed to initialize Chromium (CEF)");
  }
  self->browser.signals.env_ready().emit();
};

void Chromium::create_browser_controller_and_core() {
  if (!gbw.windows_are_ready()) {
    gbw.signals.windows_are_ready().connect(
        [this] { create_browser_controller_and_core(); });
    return;
  }
  CefWindowInfo window_info;
  CefBrowserSettings browser_settings;

  auto hwnd = os.window.browser();
  window_info.SetAsChild(reinterpret_cast<CefWindowHandle>(hwnd));

  if (!CefBrowserHost::CreateBrowser(window_info, self->browser.api.client(),"about:blank", browser_settings, nullptr)) {
    throw gbw_error("Failed to create Chromium (CEF) browser");
  }
  self->browser.signals.controller_ready().emit();
}
