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

#include "core/lib/Lib_gbw.hh"
#include "Browser_widget.hh" // IWYU pragma: keep

using namespace gbw::core::lib;

/* #region Lib_gbw */
Lib_gbw::~Lib_gbw() {}

Lib_gbw::Lib_gbw(gbw_widget_t *widget, gbw_browser_t *engine)
    : widget(widget), engine(engine), gbw(this) {};

position_t Lib_gbw::csd_fudge{0, 0};

/* #endregion */
/* #region Lib_gbw::gbw::layout */

Lib_gbw::gbw_api_layout_t::gbw_api_layout_t(Lib_gbw *self)
    : nested_api_t(self) {};

layout_t Lib_gbw::gbw_api_layout_t::get_rel() {

  auto bounds = self->widget->compute_bounds(*self->gbw.window.top_level());

  int x = bounds->get_x();
  int y = bounds->get_y();
  int width = bounds->get_width();
  int height = bounds->get_height();

  return {x, y, width, height};
}

layout_t Lib_gbw::gbw_api_layout_t::get_new() {
  auto native_window = self->os.window.top_level();
  auto rel_layout = self->gbw.layout.get_rel();
  auto abs_layout = self->os.layout.get(native_window);
  auto csd_offset = self->gbw.csd.get_offset(abs_layout);
  auto x = rel_layout.x + abs_layout.x + csd_offset.x;
  auto y = rel_layout.y + abs_layout.y + csd_offset.y;
  return {x, y, rel_layout.width, rel_layout.height};
};

layout_eq_t Lib_gbw::gbw_api_layout_t::has_changed(layout_t &new_layout,
                                                   layout_t &current_layout) {
  auto origin_changed =
      (new_layout.x != current_layout.x) || (new_layout.y != current_layout.y);
  auto size_changed = (new_layout.width != current_layout.width) ||
                      (new_layout.height != current_layout.height);

  return {origin_changed, size_changed};
};

void Lib_gbw::gbw_api_layout_t::size_browser(layout_t &layout) {
  self->gbw.window.browser()->set_default_size(layout.width, layout.height);
};

/* #endregion */
/* #region Lib_gbw::gbw */

Lib_gbw::gbw_api_t::gbw_api_t(Lib_gbw *self)
    : nested_api_t(self), window(self), csd(self), layout(self), signals(self) {
}

bool Lib_gbw::gbw_api_t::windows_are_ready() {
  return self->windows_are_ready;
};

void Lib_gbw::gbw_api_t::windows_are_ready(bool flag) {
  self->windows_are_ready = flag;
};

/* #endregion */
/* #region Lib_gbw::gbw::window */

Lib_gbw::gbw_api_window_t::gbw_api_window_t(Lib_gbw *self)
    : nested_api_t(self) {};

gtk_window_t *Lib_gbw::gbw_api_window_t::top_level() {
  if (!self->top_level_window) {
    auto root_ptr = self->widget->get_root();
    auto window = dynamic_cast<gtk_window_t *>(root_ptr);
    self->top_level_window = window;
  }
  return self->top_level_window;
};

gtk_window_t *Lib_gbw::gbw_api_window_t::browser() {
  if (!self->browser_window) {
    self->browser_window = static_cast<gtk_window_t *>(self->engine);
  }
  return self->browser_window;
};

/* #endregion */
/* #region Lib_gbw::gbw::csd */

bool Lib_gbw::gbw_api_csd_t::is_active() {
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

Lib_gbw::gbw_api_csd_t::gbw_api_csd_t(Lib_gbw *self) : nested_api_t(self) {};

position_t &Lib_gbw::gbw_api_csd_t::fudge() { return self->csd_fudge; }

position_t Lib_gbw::gbw_api_csd_t::get_offset(layout_t native) {
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
/* #region Lib_gbw::gbw::signals */

Lib_gbw::gbw_api_signals_t::gbw_api_signals_t(Lib_gbw *self)
    : nested_api_t(self) {}

ready_signal_t Lib_gbw::gbw_api_signals_t::windows_are_ready() {
  return self->windows_are_ready_signal;
}

/* #endregion */
void Lib_gbw::fudge(int x, int y) { Lib_gbw::csd_fudge = {x, y}; };

// position_t Windows::win_api_window_t::csd_decorations_offset_imp(
//     HWND &hWnd, gtk_window_t &window, position_t &fudge) {
//   if (window.is_maximized() || !is_using_gtk_csd(window)) {
//     return {0, 0};
//   }
//   // Get the HWND browser window size in scaled pixels
//   RECT rect;
//   GetWindowRect(hWnd, &rect);
//   auto scale = native_window_dpi_scale_imp(hWnd);
//   float wr_w = (rect.right - rect.left) * scale;
//   float wr_h = (rect.bottom - rect.top) * scale;
//   // Get the Gtk::Window browser size
//   auto tl_w = window.get_allocated_width();
//   auto tl_h = window.get_allocated_height();
//   // Half the total decoration size in each direction gives us a best
//   estimate
//   // additional xy origin adjustment. We may still need to apply an
//   additional
//   // user provided fudge depending on the asymmetary of any given theme.
//   int dec_x = floor(float(wr_w - tl_w) / 2) + fudge.x;
//   int dec_y = floor(float(wr_h - tl_h) / 2) + fudge.y;
//
//   return {dec_x, dec_y};
// }

/* #endregion */
