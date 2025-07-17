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
