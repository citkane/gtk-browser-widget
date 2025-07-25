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

#ifndef GBW_CORE_GBW_BASE_HH
#define GBW_CORE_GBW_BASE_HH

#include "os/Os.hh"
#include "types/types.hh"

namespace gbw {
namespace core::lib {

class Gbw_base : protected Os {

public:
  virtual ~Gbw_base() = 0;
  Gbw_base(gbw_widget_t *widget, gbw_browser_t *engine);

  /// Set the fudge offset to compensate for pixel perfect alignment
  /// @see gbw::options::gtk::csd::set_fudge
  static void fudge(int x, int y);

private:
  struct gbw_api_layout_t : nested_api_t<Gbw_base> {
    gbw_api_layout_t(Gbw_base *self);

    /// Returns the relative layout of the browser widget to the top level
    /// Gtk::Window
    layout_t get_rel();

    /// Returns the layout of the intended position of the native browser window
    /// relative to the screen
    layout_t get_new();

    /// Updates the browser window with the new layout;
    void update(layout_t &new_layout);

    /// Calculates if the layout origin or size has changed
    layout_eq_t has_changed(layout_t &new_layout, layout_t &current_layout);

    /// Sizes the browser window to the layout
    void size_browser(layout_t &layout);
  };

  struct gbw_api_window_t : nested_api_t<Gbw_base> {
    gbw_api_window_t(Gbw_base *self);

    /// Gets the top level Gtk::Window
    gtk_window_t *top_level();

    /// Gets the browser Gtk::Window
    gtk_window_t *browser();
  };

  struct gbw_api_csd_t : nested_api_t<Gbw_base> {
    gbw_api_csd_t(Gbw_base *self);

    /// Determines if the given `Gtk::Window` is using gtk Client Side
    /// Decorations (CSD)
    /// @returns `true` if CSD is in use or `false` if the OS window
    /// decorator is in use.
    bool is_active();

    /// Get the fudge offset that compensates for pixel perfect alignment
    /// @see gbw::options::gtk::csd::set_fudge
    position_t &fudge();

    /// For CSD managed Gtk::Windows, we need to positionally compensate for the
    /// GTK decorations which are wrapped inside of the HWND measurement.
    /// @param native The native top level window layout
    position_t get_offset(layout_t native);
  };

  struct gbw_api_signals_t : nested_api_t<Gbw_base> {
    gbw_api_signals_t(Gbw_base *self);

    ready_signal_t windows_are_ready();
  };

  struct gbw_api_t : private nested_api_t<Gbw_base> {
    gbw_api_t(Gbw_base *self);
    /// Gtk::Window related operations
    gbw_api_window_t window;
    /// Gtk CSD (Client Side Decoration) related operations
    gbw_api_csd_t csd;
    /// GBW window layout related operations
    gbw_api_layout_t layout;
    /// GBW signal related operations
    gbw_api_signals_t signals;

    bool windows_are_ready();
    void windows_are_ready(bool flag);
  };

protected:
  /// Gbw API root
  gbw_api_t gbw;

private:
  static position_t csd_fudge;

  gbw_widget_t *widget;
  gbw_browser_t *engine;
  gtk_window_t *top_level_window = nullptr;
  gtk_window_t *browser_window = nullptr;

  ready_signal_t windows_are_ready_signal;
  bool windows_are_ready{};

  friend class gbw::Gbw_widget;
  friend struct gbw_api_t;
  friend struct gbw_api_window_t;
  friend struct gbw_api_csd_t;
};

} // namespace core::lib

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
    static void try_bypass() { g_setenv("GTK_CSD", "0", TRUE); }

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
    static void set_fudge(int x, int y) { core::lib::Gbw_base::fudge(x, y); }
  };
};

} // namespace options
} // namespace gbw

#endif // GBW_CORE_GBW_BASE_HH
