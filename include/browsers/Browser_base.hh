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
  Browser_base() = default;

protected:
  struct api_layout_t : nested_api_t<Browser> {
    api_layout_t(Browser *self);

    /// Fits the browser's rendered window to the Gtk::Window
    virtual void fit(layout_t &layout) = 0;
  };

  struct api_signals_t : nested_api_t<Browser_base> {
    api_signals_t(Browser_base *self);

    /// The signal for the browser core API ready event.
    ready_signal_t &core_ready();

    /// The signal for the browser environment API ready event.
    ready_signal_t &env_ready();

    /// The signal for the browser controller API ready event.
    ready_signal_t &controller_ready();
  };

  struct api_api_t : nested_api_t<Browser_base> {
    api_api_t(Browser_base *self);

    /// Gets the browser core API (ie. the window instance DOM/JS API)
    smart_core_t &core();

    /// Gets the browser controller API (ie. the browser window settings)
    smart_control_t &controller();

    /// Gets the browser environment API (ie. the browser engine environment)
    smart_env_t &environment();

    /// Flag for if all browser API's are loaded and ready
    bool ready();
  };

  struct api_t : nested_api_t<Browser> {
    api_t(Browser *self);

    /// Initialises the browser engine bootstrap sequence.
    virtual void init() = 0;

    /// Browser API branch for working with layout
    api_layout_t &layout;
    /// Browser API branch for working with signals
    api_signals_t signals;
    /// Browser api branch for working with browser engine API's
    api_api_t api;
  };

  /// Browser api_environment API mutator
  void set_api_environment(browser_env_t &env);
  /// Browser api_controller API mutator
  void set_api_controller(browser_controller_t &controller);
  /// Browser api_core API mutator
  void set_api_core(browser_core_t &core);

private:
  smart_env_t api_environment;
  smart_control_t api_controller;
  smart_core_t api_core;

  ready_signal_t environment_ready_signal;
  ready_signal_t controller_ready_signal;
  ready_signal_t core_ready_signal;

  bool api_is_ready{};

  friend gbw::Browser;
  friend struct api_signals_t;
};

} // namespace gbw::browsers

#endif // GBW_BROWSERS_BROWSER_BASE_HH
