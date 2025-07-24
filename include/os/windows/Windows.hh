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

#include "os/lib/Lib_os.hh"
#include "types/types.hh"

namespace gbw {
namespace os::windows {

/// @brief Library for interfacing with Windows OS system API.
///
/// @details We don't want to marry ourselves to Microsoft and the MSVC
/// compiler, so replacement logic is provided for the `wil` and `wrl` framework
/// MSVC only methods:
/// - @ref gbw::os::windows::smart_ptr
/// - @ref gbw::os::windows::Callback_handler
/// @ref os/windows/types/types_winos.hh
class Windows : public Lib_os {
public:
  virtual ~Windows() = 0;
  Windows() : os(this), api_window(this), api_layout(this) {};

  static void attach_win32_console();

private:
  struct win_api_layout_t : public os_api_layout_t {
    win_api_layout_t(Windows *self);

    layout_t get(native_window_t &native_window) override;

    position_t get_position(native_window_t &native_window,
                            float dpi_scale) override;

    dimension_t get_size(native_window_t &native_window,
                         float dpi_scale) override;

    void move(native_window_t &native_window, layout_t &new_layout) override;
  };

  struct win_api_window_t : public os_api_window_t {
    win_api_window_t(Windows *self) : os_api_window_t(self) {};

    native_window_t convert_gtk_to_native(gtk_window_t &window) override;

    float get_dpi_scale(native_window_t &native_window) override;

    native_window_t top_level() override;

    native_window_t browser() override;
  };

  struct win_api_t : public os_api_t {
    win_api_t(Windows *self);

    void set_native_windows(gtk_window_t &top_level,
                            gtk_window_t &browser) override;
  };

protected:
  /// The gwb::os API root
  win_api_t os;

private:
  static bool is_console_attached;

  win_api_window_t api_window;
  win_api_layout_t api_layout;
  // win_api_signals_t win_api_signals;

  friend struct Lib_os::os_api_t::os_api_t;

}; // namespace gbw

} // namespace os::windows

namespace options {

/// Attach the windows console to the parent GUI process so that we can get
/// debug output
inline void attach_win32_console() {
  os::windows::Windows::attach_win32_console();
}

} // namespace options
} // namespace gbw

#endif // _WIN32
#endif // GBW_OS_WINDOWS_WINDOWS_HH
