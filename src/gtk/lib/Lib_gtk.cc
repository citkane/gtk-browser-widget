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

using namespace gbw;
using namespace gbw::gtk;

bool Lib_gtk::is_using_gtk_csd(gtk_window_t &window) {
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

void Lib_gtk::set_csd_fudge(int x, int y) { csd_fudge = {x, y}; };
offset_t &Lib_gtk::get_csd_fudge() { return csd_fudge; }
offset_t Lib_gtk::csd_fudge{0, 0};
