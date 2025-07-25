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

#include "core/Gbw_base.hh"

#include "browsers/mswebview2/lib/Controller.hh"
#include "browsers/mswebview2/lib/Environment.hh"

using namespace gbw::core;
namespace gbw::browsers {

class Mswebview2 : mswebview2::Environment,
                   mswebview2::Controller,
                   protected Gbw_base {
public:
  virtual ~Mswebview2() = 0;
  Mswebview2(gbw_widget_t *widget, gbw_browser_t *engine);

private:
  struct api_t : Browser_base::api_t {
    api_t(Mswebview2 *self);
    /// Initialises the bootstrapping of the MsWebview2 browser engine
    void init() override;
  };

  struct api_layout_t : Browser_base::api_layout_t {
    api_layout_t(Mswebview2 *self);
    /// Fits the MsWebview2 browser window into it's Gtk::Window parent
    void fit(layout_t &layout) override;
  };

protected:
  /// API root for the MsWebview2 browser
  api_t browser;

private:
  api_layout_t layout_api;

  void set_browser_env(browser_env_t &environment);
  void set_browser_controller(browser_controller_t &controller);
  void set_browser_core(browser_core_t &core);
  void create_browser_controller_and_core();

  friend class mswebview2::Environment;
  friend class mswebview2::Controller;
  friend class Browser_base;
};

} // namespace gbw::browsers

#endif // _WIN32
#endif // GBW_BROWSERS_MSWEBVIEW2_MS_WEBVIEW2_HH
