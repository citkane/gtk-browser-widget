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

#ifndef EBW_GTK_LIB_LIB_GTK_HH
#define EBW_GTK_LIB_LIB_GTK_HH

#include "types/types.hh"

namespace gbw {
namespace gtk {
class Lib_gtk;

struct gtk_t : nested_api_t<Lib_gtk> {
  gtk_t(Lib_gtk *self) : nested_api_t(self) {}

  struct window_t;
  struct csd_t;
};

struct gtk_t::window_t : nested_api_t<Lib_gtk> {
  window_t(Lib_gtk *self) : nested_api_t(self) {}

  /// Gets the top level Gtk::Window
  gtk_window_t &top_level();

  /// Gets the browser Gtk::Window
  gtk_window_t &browser();
};

struct gtk_t::csd_t : nested_api_t<Lib_gtk> {
  csd_t(Lib_gtk *self) : nested_api_t(self) {}
  /// Determines if the given `Gtk::Window` is using gtk Client Side
  /// Decorations (CSD)
  /// @returns `true` if CSD is in use or `false` if the OS window
  /// decorator is in use.
  bool is_active();

  /// Get the fudge offset that compensates for pixel perfect alignment
  /// @see gbw::options::gtk::csd::set_fudge
  offset_t &fudge();

  /// Set the fudge offset to compensate for pixel perfect alignment
  /// @see gbw::options::gtk::csd::set_fudge
  static void fudge(int x, int y);

  /// For CSD managed Gtk::Windows, we need to positionally compensate for the
  /// GTK decorations which are wrapped inside of the HWND measurement.
  /// @param native_window The OS specific top level native window
  offset_t get_offset(size_t &native_size);
};

class Lib_gtk {
public:
  Lib_gtk(gbw_widget_t *widget, gbw_browser_t *browser)
      : widget(widget), browser(browser), gtk(this), window(this), csd(this) {};

protected:
  /// gbw Gtk api
  gtk_t gtk;
  /// gbw Gtk:Window api
  gtk_t::window_t window;
  /// gbw Gtk Client Side Decorations (CSD) api
  gtk_t::csd_t csd;

  /// Converts a `Gtk::Window` to an OS native window
  /// @throws exception if conversion fails.
  virtual native_window_t native_window_from_gtk_imp(gtk_window_t &window) = 0;

  /// Gets the dpi scale for the given native window
  virtual float native_window_dpi_scale_imp(native_window_t &native_window) = 0;

private:
  static offset_t csd_fudge;

  gbw_widget_t *widget;
  gbw_browser_t *browser;

  friend struct gtk_t;
  friend struct gtk_t::window_t;
};

} // namespace gtk

namespace options {
struct gtk {
  /// GTK Client Side Decorations (CSD)
  struct csd {
    /// @brief Turn off GTK Client Side Decorations (CSD) and fall back to
    /// default OS decorations.
    ///
    /// @details GTK will render outer window decorations (title bar, borders,
    /// etc.) according to it's own or user defined theming. By setting this
    /// option, it will try to use the native OS theming instead. This does NOT
    /// change internal theming, eg. buttons, menus, etc.
    ///
    /// @note The fallback will be ignored if GTK widgets such as
    /// `Gtk::HeaderBar` have been explicitly set.
    inline void try_bypass() { g_setenv("GTK_CSD", "0", TRUE); }

    /// @brief Set a px positional offset fudge for pseudo-embedding the browser
    /// window
    ///
    /// @details The positioning of the browser window depends on calculating
    /// the difference between it's gtk window size and native window size,
    /// which wraps around gtk Client Side Decorations (CSD).
    ///
    /// This may be asymmetrical and different for any given theme, so the user
    /// may need to do some manual fine tuning (fudge) for their particular case
    /// to be pixel perfect.
    inline void set_fudge(int x, int y) { gbw::gtk::gtk_t::csd_t::fudge(x, y); }
  };
};

} // namespace options
} // namespace gbw

#endif // EBW_GTK_LIB_LIB_GTK_HH
