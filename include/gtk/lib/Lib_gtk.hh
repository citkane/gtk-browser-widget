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

class Lib_gtk {
public:
  /// @copydoc gbw::set_gtk_csd_fudge
  static void set_csd_fudge(int x, int y);

protected:
  /// Determines if the given `Gtk::Window` is using gtk Client Side Decorations
  /// (CSD)
  /// @returns `true` if CSD is in use or `false` if the OS window decorator is
  /// in use.
  bool is_using_gtk_csd(gtk_window_t &window);

  offset_t &get_csd_fudge();

private:
  static offset_t csd_fudge;
};

} // namespace gtk

namespace options {

/// @brief Turn off GTK Client Side Decorations (CSD) and fall back to default
/// OS decorations.
///
/// @details GTK will render outer window decorations (title bar, borders, etc.)
/// according to it's own or user defined theming. By setting this option, it
/// will try to use the native OS theming instead. This does NOT change internal
/// theming, eg. buttons, menus, etc.
///
/// @note The fallback will be ignored if GTK widgets such as `Gtk::HeaderBar`
/// have been explicitly set.
inline void try_bypass_gtk_csd() { g_setenv("GTK_CSD", "0", TRUE); }

/// @brief Set a px positional offset fudge for pseudo-embedding the browser
/// window
///
/// @details The positioning of the browser window depends on calculating the
/// difference between it's gtk window size and native window size, which wraps
/// around gtk Client Side Decorations (CSD).
///
/// This may be assymetrical and different for any given theme, so the user may
/// need to do some manual fine tuning (fudge) for their particular case to be
/// pixel perfect.
inline void set_gtk_csd_fudge(int x, int y) {
  gbw::gtk::Lib_gtk::set_csd_fudge(x, y);
}

} // namespace options
} // namespace gbw

#endif // EBW_GTK_LIB_LIB_GTK_HH
