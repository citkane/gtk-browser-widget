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

#include "Browser_widget.hh" // IWYU pragma: keep
#include "gtk/Browser_window.hh"
#include "types/types.hh"

using namespace gbw::gtk;

Browser_window::~Browser_window() {}

Browser_window::Browser_window(ebw_widget_t &widget) : browser_widget(&widget) {
  Gtk::manage(this);

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
  std::cout << "Browser window is realised\n";
  Glib::signal_idle().connect_once([this] { window_ready_cb(); });
}

void Browser_window::window_ready_cb() {
  std::cout << "Browser window is ready\n";
  auto root_ptr = browser_widget->get_root();
  top_level_window = dynamic_cast<gtk_window_t *>(root_ptr);
  if (!top_level_window) {
    throw ebw_error("Failed to get the top level gtk::Window");
  }

  set_focus_controller();
  set_transient_for(*top_level_window);
  set_native_windows_impl(*this, *top_level_window);

  browser_engine_init_impl();
  browser_ready_impl().connect([this] {
    set_visible(true);
    present();
  });
}
// Dummy trigger for code review 2025-07-18
void Browser_window::set_focus_controller() {
  auto focus_controller = Gtk::EventControllerLegacy::create();
  add_controller(focus_controller);

  focus_controller->signal_event().connect(
      [this](gdk_event_t &event) {
        auto focus_changed = event->get_event_type() == FOCUS_CHANGE;
        if (!focus_changed) {
          return true;
        }
        top_level_window->present();
        return false;
      },
      true);
};
