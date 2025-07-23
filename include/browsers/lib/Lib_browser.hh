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

#ifndef GBW_BROWSERS_LIB_LIB_BROWSER_HH
#define GBW_BROWSERS_LIB_LIB_BROWSER_HH

#include "types/types.hh"

namespace gbw::browsers {

class Lib_browser {
public:
  virtual ~Lib_browser() = 0;

protected:
  struct browser_api_layout_t : nested_api_t<Browser_engine> {
    browser_api_layout_t(Browser_engine *self) : nested_api_t(self) {}

    /// Gets the browser core API (ie. the window instance DOM/JS API)
    virtual void fit(layout_t &layout) = 0;
  };

  struct browser_api_signals_t : nested_api_t<Browser_engine> {
    browser_api_signals_t(Browser_engine *self) : nested_api_t(self) {}

    /// The signal for the browser window ready event.
    virtual ready_signal_t &core_ready() = 0;

    /// The signal for the browser environment ready event.
    virtual ready_signal_t &env_ready() = 0;

    /// The signal for the browser controller ready event.
    virtual ready_signal_t &controller_ready() = 0;
  };

  struct browser_api_api_t : nested_api_t<Browser_engine> {
    browser_api_api_t(Browser_engine *self) : nested_api_t(self) {}

    /// Gets the browser core API (ie. the window instance DOM/JS API)
    virtual smart_core_t &core() = 0;

    /// Gets the browser controller API (ie. the browser engine settings)
    virtual smart_control_t &controller() = 0;

    /// Gets the browser environment API (ie. the browser engine environment)
    virtual smart_env_t &environment() = 0;
  };

  struct browser_api_t : nested_api_t<Browser_engine> {
    browser_api_t(Browser_engine *self, browser_api_api_t *api,
                  browser_api_layout_t *layout, browser_api_signals_t *signals)
        : nested_api_t(self), api(*api), layout(*layout), signals(*signals) {};

    /// Initialises the browser engine and waits for completion signals.
    virtual void init() = 0;

    /// Browser API branch for working directly with the browser engine API's
    browser_api_api_t &api;

    /// Browser API branch for working with layout
    browser_api_layout_t &layout;

    /// Browser API branch for working with api ready signals
    browser_api_signals_t &signals;
  };

  ready_signal_t &get_environment_ready_signal() {
    return environment_ready_signal;
  };
  ready_signal_t &get_controller_ready_signal() {
    return controller_ready_signal;
  };
  ready_signal_t &get_core_ready_signal() { return core_ready_signal; };

  smart_env_t &get_api_environment() { return api_environment; }
  smart_control_t &get_api_controller() { return api_controller; }
  smart_core_t &get_api_core() { return api_core; }

  void set_api_environment(browser_env_t &env) {
    api_environment = smart_env_t(&env);
  }
  void set_api_controller(browser_controller_t &controller) {
    api_controller = smart_control_t(&controller);
  }
  void set_api_core(browser_core_t &core) { api_core = smart_core_t(&core); }

private:
  gdk_bounds_t bounds;

  smart_env_t api_environment = nullptr;
  smart_control_t api_controller = nullptr;
  smart_core_t api_core = nullptr;

  ready_signal_t environment_ready_signal;
  ready_signal_t controller_ready_signal;
  ready_signal_t core_ready_signal;

  friend gbw::Browser_engine;
};

inline Lib_browser::~Lib_browser() {}

} // namespace gbw::browsers

#endif // GBW_BROWSERS_LIB_LIB_BROWSER_HH