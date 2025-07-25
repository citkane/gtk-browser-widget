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

#include "core/Gbw_base.hh"
#include "Gbw_widget.hh" // IWYU pragma: keep

using namespace gbw::core;

/* #region Gbw_base */
Gbw_base::~Gbw_base() {}

Gbw_base::Gbw_base(gbw_widget_t *widget, gbw_browser_t *engine)
    : widget(widget), engine(engine), gbw(this) {};

position_t Gbw_base::csd_fudge{0, 0};

/* #endregion */
/* #region Gbw_base::api_layout_t */

Gbw_base::api_layout_t::api_layout_t(Gbw_base *self) : nested_api_t(self) {};

layout_t Gbw_base::api_layout_t::get_rel() {

  auto bounds = self->widget->compute_bounds(*self->gbw.window.top_level());

  int x = bounds->get_x();
  int y = bounds->get_y();
  int width = bounds->get_width();
  int height = bounds->get_height();

  return {x, y, width, height};
}

layout_t Gbw_base::api_layout_t::get_new() {
  auto native_window = self->os.window.top_level();
  auto rel_layout = self->gbw.layout.get_rel();
  auto abs_layout = self->os.layout.get(native_window);
  auto csd_offset = self->gbw.csd.get_offset(abs_layout);
  auto x = rel_layout.x + abs_layout.x + csd_offset.x;
  auto y = rel_layout.y + abs_layout.y + csd_offset.y;
  return {x, y, rel_layout.width, rel_layout.height};
};

layout_eq_t Gbw_base::api_layout_t::has_changed(layout_t &new_layout,
                                                layout_t &current_layout) {
  auto origin_changed =
      (new_layout.x != current_layout.x) || (new_layout.y != current_layout.y);
  auto size_changed = (new_layout.width != current_layout.width) ||
                      (new_layout.height != current_layout.height);

  return {origin_changed, size_changed};
};

void Gbw_base::api_layout_t::size_browser(layout_t &layout) {
  self->gbw.window.browser()->set_default_size(layout.width, layout.height);
};

/* #endregion */
/* #region Gbw_base::api_t */

Gbw_base::api_t::api_t(Gbw_base *self)
    : nested_api_t(self), window(self), csd(self), layout(self), signals(self) {
}

bool Gbw_base::api_t::windows_are_ready() { return self->windows_are_ready; };

void Gbw_base::api_t::windows_are_ready(bool flag) {
  self->windows_are_ready = flag;
};

/* #endregion */
/* #region Gbw_base::api_window_t */

Gbw_base::api_window_t::api_window_t(Gbw_base *self) : nested_api_t(self) {};

gtk_window_t *Gbw_base::api_window_t::top_level() {
  if (!self->top_level_window) {
    auto root_ptr = self->widget->get_root();
    auto window = dynamic_cast<gtk_window_t *>(root_ptr);
    self->top_level_window = window;
  }
  return self->top_level_window;
};

gtk_window_t *Gbw_base::api_window_t::browser() {
  if (!self->browser_window) {
    self->browser_window = static_cast<gtk_window_t *>(self->engine);
  }
  return self->browser_window;
};

/* #endregion */
/* #region Gbw_base::api_csd_t */

bool Gbw_base::api_csd_t::is_active() {
  auto window = self->gbw.window.top_level();
  auto is_application = !!window->get_application();
  auto has_titlebar = !!window->get_titlebar();
  auto csd_env = std::getenv("GTK_CSD");
  auto csd_env_disabled = !!csd_env && std::string(csd_env) == "0";
  auto undecorated = !window->get_decorated();

  // std::cout << "is_application: " << (is_application ? "true" : "false")
  //           << "\n";
  // std::cout << "has_titlebar: " << (has_titlebar ? "true" : "false") << "\n";
  // std::cout << "csd_env_disabled: " << (csd_env_disabled ? "true" : "false")
  //           << "\n";
  // std::cout << "undecorated: " << (undecorated ? "true" : "false") << "\n";

  if (undecorated || (csd_env_disabled && !has_titlebar)) {
    return false;
  }
  return true;
}

Gbw_base::api_csd_t::api_csd_t(Gbw_base *self) : nested_api_t(self) {};

position_t &Gbw_base::api_csd_t::fudge() { return self->csd_fudge; }

position_t Gbw_base::api_csd_t::get_offset(layout_t native) {
  auto gtk = self->gbw.window.top_level();
  if (gtk->is_maximized() || !gtk->is_active()) {
    return {0, 0};
  }

  auto gtk_width = gtk->get_allocated_width();
  auto gtk_height = gtk->get_allocated_height();

  // Half the total decoration size in each direction gives us a best estimate
  // xy origin adjustment. We may still need to apply an additional
  // user provided fudge depending on the asymmetry of any given theme.
  int x = floor(float(native.width - gtk_width) / 2) + self->csd_fudge.x;
  int y = floor(float(native.height - gtk_height) / 2) + self->csd_fudge.y;

  return {x, y};
}

/* #endregion */
/* #region Gbw_base::api_signals_t */

Gbw_base::api_signals_t::api_signals_t(Gbw_base *self) : nested_api_t(self) {}

ready_signal_t Gbw_base::api_signals_t::windows_are_ready() {
  return self->windows_are_ready_signal;
}

/* #endregion */

void Gbw_base::fudge(int x, int y) { Gbw_base::csd_fudge = {x, y}; };
