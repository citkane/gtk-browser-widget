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

#include "gtk/lib/Lib_gtk.hh"
#include "Browser_widget.hh" // IWYU pragma: keep

using namespace gbw;
using namespace gbw::gtk;

bool gtk_t::csd_t::is_active() {
  auto &window = self->window.top_level();
  auto is_application = !!window.get_application();
  auto has_titlebar = !!window.get_titlebar();
  auto csd_env = std::getenv("GTK_CSD");
  auto csd_env_disabled = !!csd_env && std::string(csd_env) == "0";
  auto undecorated = !window.get_decorated();

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
  // return has_titlebar;
}

gtk_window_t &gtk_t::window_t::top_level() {
  return self->widget->get_top_level_window();
};

gtk_window_t &gtk_t::window_t::browser() {
  gtk_window_t *browser_ = self->browser;
  return *browser_;
};

offset_t &gtk_t::csd_t::fudge() { return self->csd_fudge; }

offset_t gtk_t::csd_t::get_offset(size_t &native_size) {
  auto &window = self->window.top_level();
  if (window.is_maximized() || !self->csd.is_active()) {
    return {0, 0};
  }
  // Get the HWND browser window size in scaled pixels
  // RECT rect;
  // GetWindowRect(hWnd, &rect);
  // auto scale = native_window_dpi_scale_imp(hWnd);
  // float wr_w = (rect.right - rect.left) * scale;
  // float wr_h = (rect.bottom - rect.top) * scale;
  // Get the Gtk::Window browser size
  auto tl_w = window.get_allocated_width();
  auto tl_h = window.get_allocated_height();
  // Half the total decoration size in each direction gives us a best estimate
  // additional xy origin adjustment. We may still need to apply an additional
  // user provided fudge depending on the asymmetary of any given theme.
  int dec_x = floor(float(native_size.width - tl_w) / 2) + self->csd_fudge.x;
  int dec_y = floor(float(native_size.height - tl_h) / 2) + self->csd_fudge.y;

  return {dec_x, dec_y};
}

void gtk_t::csd_t::fudge(int x, int y) { Lib_gtk::csd_fudge = {x, y}; };

offset_t Lib_gtk::csd_fudge{0, 0};
