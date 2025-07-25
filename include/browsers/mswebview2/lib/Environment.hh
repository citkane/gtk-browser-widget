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

#ifndef GBW_BROWSERS_MSWEBVIEW2_LIB_ENVIRONMENT_HH
#define GBW_BROWSERS_MSWEBVIEW2_LIB_ENVIRONMENT_HH

#include "types/types.hh" // IWYU pragma: keep

namespace gbw::browsers {
class Mswebview2;

namespace mswebview2 {

/// MsWebview HWND evironment creation and access
class Environment {

protected:
  /// Callback class that provides the `ICoreWebView2Environment` for a
  /// `HWND` native window
  /// @see
  /// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2createcorewebview2environmentcompletedhandler?view=webview2-1.0.3351.48
  class CompletedHandler;

private:
  using Cb_t = ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler;
  using Cb_handler = Callback_handler<Cb_t>;
};

class Environment::CompletedHandler : public Cb_handler {
public:
  CompletedHandler(Mswebview2 &webview) : webview(&webview) {};

  HRESULT STDMETHODCALLTYPE Invoke(HRESULT result, browser_env_t *env) override;

private:
  Mswebview2 *webview;
};

} // namespace mswebview2
} // namespace gbw::browsers
#endif // GBW_BROWSERS_MSWEBVIEW2_LIB_ENVIRONMENT_HH
