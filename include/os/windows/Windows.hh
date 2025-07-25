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

#ifndef GBW_OS_WINDOWS_WINDOWS_HH
#define GBW_OS_WINDOWS_WINDOWS_HH
#ifdef _WIN32

#include "os/Os_base.hh"
#include "types/types.hh"

namespace gbw {
namespace os {

class Windows : public Os_base {
public:
  virtual ~Windows() = 0;
  Windows() : os(this), api_window(this), api_layout(this) {};

  /// Utility function to attach the console output to the Windows GUI app
  /// process,
  static void attach_win32_console();

private:
  struct api_layout_t : public Os_base::api_layout_t {
    api_layout_t(Windows *self);
    /// Gets the pixel layout for the given HWND relative to the screen
    layout_t get(native_window_t &native_window) override;
    /// Gets the top left pixel position for the given HWND relative to
    /// the screen
    position_t get_position(native_window_t &native_window,
                            float dpi_scale) override;
    /// Gets pixel size for the given HWND
    dimension_t get_size(native_window_t &native_window,
                         float dpi_scale) override;
    /// Moves the given HWND to the new pixel origin relative to the
    /// screen
    void move(native_window_t &native_window, layout_t &new_layout) override;
  };

  struct api_window_t : public Os_base::api_window_t {
    api_window_t(Windows *self);

    /// Converts a `Gtk::Window` to an HWND
    native_window_t convert_gtk_to_native(gtk_window_t &window) override;
    /// Gets the dpi scale for the given HWND
    float get_dpi_scale(native_window_t &native_window) override;
    /// Get the top level HWND object
    native_window_t top_level() override;
    /// Get the browser HWND object
    native_window_t browser() override;
  };

  struct api_t : public Os_base::api_t {
    api_t(Windows *self);

    /// Mutator that converts and sets the top level and browser `Gtk::Window`'s
    /// to HWND's
    void set_native_windows(gtk_window_t &top_level,
                            gtk_window_t &browser) override;
  };

protected:
  /// The gwb::os API root
  api_t os;

private:
  static bool is_console_attached;

  api_window_t api_window;
  api_layout_t api_layout;

  friend struct Os_base::api_t::api_t;
};

} // namespace os

namespace options {
/// Static global MS Windows options
struct win {
  /// Attach the windows console to the parent GUI process so that we can get
  /// debug output
  static void attach_win32_console() { os::Windows::attach_win32_console(); }
};
} // namespace options
} // namespace gbw

#endif // _WIN32
#endif // GBW_OS_WINDOWS_WINDOWS_HH
