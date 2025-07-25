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

#ifndef GBW_BROWSERS_BROWSER_BASE_HH
#define GBW_BROWSERS_BROWSER_BASE_HH

#include "types/types.hh"

namespace gbw::browsers {

class Browser_base {
public:
  virtual ~Browser_base() = 0;
  Browser_base() {}

protected:
  struct browser_api_layout_t : nested_api_t<Browser> {
    browser_api_layout_t(Browser *self);

    virtual void fit(layout_t &layout) = 0;
  };

  struct browser_api_signals_t : nested_api_t<Browser_base> {
    browser_api_signals_t(Browser_base *self);

    /// The signal for the browser window ready event.
    ready_signal_t &core_ready();

    /// The signal for the browser environment ready event.
    ready_signal_t &env_ready();

    /// The signal for the browser controller ready event.
    ready_signal_t &controller_ready();
  };

  struct browser_api_api_t : nested_api_t<Browser_base> {
    browser_api_api_t(Browser_base *self);

    /// Gets the browser core API (ie. the window instance DOM/JS API)
    smart_core_t &core();

    /// Gets the browser controller API (ie. the browser engine settings)
    smart_control_t &controller();

    /// Gets the browser environment API (ie. the browser engine environment)
    smart_env_t &environment();

    bool ready();
  };

  struct browser_api_t : nested_api_t<Browser> {
    browser_api_t(Browser *self);

    /// Initialises the browser engine and waits for completion signals.
    virtual void init() = 0;

    /// Browser API branch for working with layout
    browser_api_layout_t &layout;

    browser_api_signals_t signals;

    browser_api_api_t api;
  };

  void set_api_environment(browser_env_t &env);
  void set_api_controller(browser_controller_t &controller);
  void set_api_core(browser_core_t &core);

private:
  gdk_bounds_t bounds;

  smart_env_t api_environment;
  smart_control_t api_controller;
  smart_core_t api_core;

  ready_signal_t environment_ready_signal;
  ready_signal_t controller_ready_signal;
  ready_signal_t core_ready_signal;

  bool api_is_ready{};

  friend gbw::Browser;
  friend struct browser_api_signals_t;
  friend class gbw::browsers::Mswebview2;
};

inline Browser_base::~Browser_base() {}

} // namespace gbw::browsers

#endif // GBW_BROWSERS_BROWSER_BASE_HH
