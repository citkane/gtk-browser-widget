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

#ifndef GBW_WIDGET_HH
#define GBW_WIDGET_HH

#include "core/Browser_window.hh"
#include "types/types.hh"

using namespace gbw::types;

using namespace gbw::core;
namespace gbw {
class Browser_widget : public gtk_widget_t {
public:
  ~Browser_widget() override {
    std::cout << "Destroying Browser_widget" << std::endl;
    engine.close();
    std::cout << "Destroyed Browser_widget" << std::endl;
  }

  Browser_widget() : engine(this) {
    Gtk::manage(this);
    init_widget_layout();
  };

  ready_signal_t &ready() { return engine.browser.signals.core_ready(); }

  auto &api_core() { return engine.browser.api.core(); }

  auto &api_controller() { return engine.browser.api.controller(); }

  auto &api_env() { return engine.browser.api.environment(); }

private:
  Browser_window engine;

  void init_widget_layout() {
    set_hexpand(true);
    set_vexpand(true);
    set_halign(Gtk::Align::FILL);
    set_valign(Gtk::Align::FILL);
    set_margin_top(0);
    set_margin_bottom(0);
    set_margin_start(0);
    set_margin_end(0);
  }
};

} // namespace gbw
#endif // GBW_WIDGET_HH
