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

#ifndef EBW_WIDGET_HH
#define EBW_WIDGET_HH

#include "gtk/Browser_engine.hh" // IWYU pragma: keep
#include "gtk/lib/Lib_gtk.hh"    // IWYU pragma: keep
#include "types/types.hh"

using namespace gbw::types;

namespace gbw {
class Browser_widget : public gtk_widget_t {
public:
  ~Browser_widget() override {
    std::cout << "Destroying Browser_widget" << std::endl;
    browser_engine.close();
    std::cout << "Destroyed Browser_widget" << std::endl;
  }

  Browser_widget() : browser_engine(*this) {
    Gtk::manage(this);
    init_widget_layout();
    browser_engine.ready_signal().connect([this] {
      std::cout << "Browser_widget got ready signal\n";
      ready_signal.emit();
    });
  };

  ready_signal_t &ready() { return ready_signal; }

  browser_api_t api() {
    return {browser_engine.outer_api(), browser_engine.inner_api()};
  }

private:
  Browser_engine browser_engine;
  ready_signal_t ready_signal;

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
#endif // EBW_WIDGET_HH
