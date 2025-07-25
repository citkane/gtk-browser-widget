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

#ifndef GBW_BROWSERS_CHROMIUM_LIB_CONTROLLER_HH
#define GBW_BROWSERS_CHROMIUM_LIB_CONTROLLER_HH

#include "browsers/chromium/types.hh"
#include "os/windows/lib/smart_templates.hh" // IWYU pragma: keep

// using namespace gbw::os::windows::lib; //CEF is not windows-specific
using namespace gbw::browsers::types;

namespace gbw::browsers {
class Chromium;

namespace chromium {

/// MsWebview controls creation and access
class Controller {

protected:
  /// Callback class that provides the CefBrowser for a CefApp
  class CompletedHandler;

private:
  using Cb_t = CefRefPtr<CefBrowser>;
};

class Controller::CompletedHandler {
  public:
    CompletedHandler(Chromium &webview) : webview(&webview) {};

    void Invoke(CefRefPtr<CefBrowser> browser);
  private:
    Chromium *webview;
  };
}
}
#endif
