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

#ifndef GBW_OS_OS_BASE_HH
#define GBW_OS_OS_BASE_HH

#include "browsers/Browser_base.hh"
#include "types/types.hh"

using namespace gbw::browsers;
namespace gbw::os {

class Os_base : protected Browser_base {

public:
  virtual ~Os_base() = 0;
  Os_base() = default;

protected:
  struct api_layout_t : nested_api_t<Os> {
    api_layout_t(Os *self);

    /// Gets the pixel layout for the given native window relative to the screen
    virtual layout_t get(native_window_t &native_window) = 0;

    /// Gets the top left pixel position for the given native window relative to
    /// the screen
    virtual position_t get_position(native_window_t &native_window,
                                    float dpi_scale) = 0;

    /// Gets pixel size for the given native window
    virtual dimension_t get_size(native_window_t &native_window,
                                 float dpi_scale) = 0;

    /// Moves the given native window to the new pixel origin relative to the
    /// screen
    virtual void move(native_window_t &native_window, layout_t &new_layout) = 0;
  };

  struct api_window_t : nested_api_t<Os> {
    api_window_t(Os *self);

    /// Converts a `Gtk::Window` to an OS native window
    virtual native_window_t convert_gtk_to_native(gtk_window_t &window) = 0;

    /// Gets the dpi scale for the given native window
    virtual float get_dpi_scale(native_window_t &native_window) = 0;

    /// Get the top level native OS window object
    virtual native_window_t top_level() = 0;

    /// Get the browser native OS window object
    virtual native_window_t browser() = 0;
  };

  struct api_signals_t : nested_api_t<Os_base> {
    api_signals_t(Os_base *self);

    /// The signal for the Os native windows ready event.
    ready_signal_t &native_windows_ready();
  };

  struct api_t : nested_api_t<Os> {
    api_t(Os *self);

    /// API root for working with OS native windows
    api_window_t &window;
    /// API root for working with OS native window layouts
    api_layout_t &layout;
    /// API root for OS related signals
    api_signals_t signals;
    /// Mutator that converts and sets the top level and browser `Gtk::Window`'s
    /// to OS native windows
    virtual void set_native_windows(gtk_window_t &top_level,
                                    gtk_window_t &browser) = 0;
  };

  /// Mutator that sets the top level and browser OS native windows
  void set_native_windows(native_window_t &top_level, native_window_t &browser);

private:
  native_window_t top_level_window;
  native_window_t browser_window;

  ready_signal_t native_windows_ready_signal;

  friend gbw::Os;
};

} // namespace gbw::os

#endif // GBW_OS_OS_BASE_HH
