#ifndef EBW_OS_WINDOWS_LIB_WIN_HH
#define EBW_OS_WINDOWS_LIB_WIN_HH
#ifdef __WIN32__

#include "gtk/lib/Lib_gtk.hh"
#include "types/types.hh"

using namespace gbw::gtk;

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
class Lib_win : protected Lib_gtk {
public:
  /// @copydoc gbw::attach_win32_console
  static void attach_win32_console();

protected:
  /// Converts a `Gtk::Window` to a Windows `HWND`
  ///
  /// @throws exception if conversion fails.
  HWND get_hWnd_from_gtk(gtk_window_t &window);

  float get_hwnd_dpi_scale(HWND &hWnd);

  // For CSD managed Gtk::Windows, we need to positionally compensate for the
  // GTK decorations which are included in the HWND measurement.
  offset_t get_decorations_offset(HWND &hWnd, gtk_window_t &window,
                                  offset_t &fudge);

private:
  static bool is_console_attached;
};

} // namespace os::windows

namespace options {
/// Attach the windows console to the parent GUI process so that we can get
/// debug output
inline void attach_win32_console() {
  os::windows::Lib_win::attach_win32_console();
}

} // namespace options
} // namespace gbw

#endif // __WIN32__
#endif // EBW_OS_WINDOWS_LIB_WIN_HH