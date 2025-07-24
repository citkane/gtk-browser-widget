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
#include <gtkmm/eventcontrollerfocus.h>
#include <gtkmm/eventcontrollerlegacy.h>

#include "core/Browser_window.hh"
#include "types/types.hh"

using namespace gbw::core;

Browser_window::~Browser_window() {
  std::cout << "Destroying Browser_window" << std::endl;
  browser.api.controller()->Close();
  std::cout << "Destroyed Browser_window" << std::endl;
}

Browser_window::Browser_window(gbw_widget_t *widget)
    : Browser_engine(widget, this) {
  Gtk::manage(this);

  browser.init();
  browser_window_init_config();
};

void Browser_window::browser_window_init_config() {
  signal_realize().connect([this] { window_realised_cb(); });

  set_default_size(0, 0);
  set_margin(0);
  set_deletable(false);
  set_destroy_with_parent(true);
  set_decorated(false);
  set_resizable(false);
  set_layout_manager(nullptr);
  set_can_focus(false);

  show();
  set_visible(false);
}

void Browser_window::window_realised_cb() {
  std::cout << "- Browser window is realised\n";
  Glib::signal_idle().connect_once([this] { window_ready_cb(); });
}

void Browser_window::window_ready_cb() {
  std::cout << "- Browser window is ready\n";
  os.set_native_windows(*gbw.window.top_level(), *gbw.window.browser());
  set_focus_controller();
  set_transient_for(*gbw.window.top_level());

  browser.signals.core_ready().connect([this] {
    set_visible(true);
    present();
    auto layout = gbw.layout.get_new();
    layout_update(layout);
    browser.api.controller()->put_IsVisible(TRUE);

    Glib::signal_idle().connect([this] {
      auto layout = gbw.layout.get_new();
      layout_update(layout);
      return true;
    });
  });

  gbw.windows_are_ready(true);
  gbw.signals.windows_are_ready().emit();
}

void Browser_window::set_focus_controller() {
  auto focus_controller = Gtk::EventControllerLegacy::create();
  add_controller(focus_controller);

  focus_controller->signal_event().connect(
      [this](gdk_event_t &event) {
        auto focus_changed = event->get_event_type() == FOCUS_CHANGE;
        if (!focus_changed) {
          return true;
        }
        gbw.window.top_level()->present();
        return false;
      },
      true);
};

void Browser_window::layout_update(layout_t &new_layout) {
  auto change = gbw.layout.has_changed(new_layout, current_layout);
  if (!change.origin && !change.size) {
    return;
  }
  auto native_window = os.window.browser();
  if (change.origin) {
    os.layout.move(native_window, new_layout);
  }
  if (change.size) {
    gbw.layout.size_browser(new_layout);
    if (!browser.api.ready()) {
      return;
    }
    browser.layout.fit(new_layout);
  }

  current_layout = new_layout;
};
