#include "os/windows/Lib_win.hh"

using namespace gbw;
using namespace gbw::os::windows;

void Lib_win::attach_win32_console() {
  if (is_console_attached) {
    return;
  }
  is_console_attached = true;
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    printf("The Windows console has been attched to the parent process.\n");
  }
}
bool Lib_win::is_console_attached{};

HWND Lib_win::get_hWnd_from_gtk(gtk_window_t &window) {
  auto surface = window.get_native()->get_surface();
  if (!surface->gobj()) {
    throw ebw_error("Failed to get surface from Gtk:Window");
  }

  auto *hWnd = GDK_SURFACE_HWND(GDK_SURFACE(surface->gobj()));
  if (!hWnd) {
    throw ebw_error("Failed to get HWND from Gdk::Surface");
  }

  return hWnd;
}

float Lib_win::get_hwnd_dpi_scale(HWND &hWnd) {
  // Try Windows 10+ API: GetDpiForWindow
  // Windows 10+ implements per monitor dpi and other features, so we can't rely
  // on the fallback for a fully integrated experience.
  HMODULE user32 = LoadLibraryW(L"user32.dll");
  if (user32) {
    typedef UINT(WINAPI * GetDpiForWindowFunc)(HWND);
    auto pGetDpiForWindow =
        (GetDpiForWindowFunc)GetProcAddress(user32, "GetDpiForWindow");
    if (pGetDpiForWindow != nullptr) {
      UINT dpi = pGetDpiForWindow(hWnd);
      if (dpi > 0) {
        FreeLibrary(user32);
        return dpi / 96.0f;
      }
    }
    FreeLibrary(user32);
  }

  // Fallback: use device context (works for all Windows)
  HDC dc = GetDC(hWnd);
  UINT dpi = 96; // Default if call fails
  if (dc) {
    dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ReleaseDC(hWnd, dc);
  }
  return dpi / 96.0f;
}

offset_t Lib_win::get_decorations_offset(HWND &hWnd, gtk_window_t &window,
                                         offset_t &fudge) {
  if (window.is_maximized() || !is_using_gtk_csd(window)) {
    return {0, 0};
  }
  // Get the HWND browser window size in scaled pixels
  RECT rect;
  GetWindowRect(hWnd, &rect);
  auto scale = get_hwnd_dpi_scale(hWnd);
  float wr_w = (rect.right - rect.left) * scale;
  float wr_h = (rect.bottom - rect.top) * scale;
  // Get the Gtk::Window browser size
  auto tl_w = window.get_allocated_width();
  auto tl_h = window.get_allocated_height();
  // Half the total decoration size in each direction gives us a best estimate
  // additional xy origin adjustment. We may still need to apply an additional
  // user provided fudge depending on the asymmetary of any given theme.
  int dec_x = floor(float(wr_w - tl_w) / 2) + fudge.x;
  int dec_y = floor(float(wr_h - tl_h) / 2) + fudge.y;

  return {dec_x, dec_y};
}
