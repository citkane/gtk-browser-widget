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

#include "os/windows/Windows.hh"

using namespace gbw;
using namespace gbw::os::windows;

Windows::~Windows() {}

/* #region os::layout */

Windows::win_api_layout_t::win_api_layout_t(Windows *self)
    : os_api_layout_t(self) {};

layout_t Windows::win_api_layout_t::get(HWND &hWnd) {
  auto dpi_scale = self->os.window.get_dpi_scale(hWnd);
  auto position = self->os.layout.get_position(hWnd, dpi_scale);
  auto size = self->os.layout.get_size(hWnd, dpi_scale);

  return {position.x, position.y, size.width, size.height};
}

void Windows::win_api_layout_t::move(HWND &native_window, layout_t &layout) {
  MoveWindow(native_window, layout.x, layout.y, layout.width, layout.width,
             FALSE);
}

position_t Windows::win_api_layout_t::get_position(HWND &hWnd,
                                                   float dpi_scale) {
  POINT top_left = {0, 0};
  ClientToScreen(hWnd, &top_left);
  int y = round(top_left.y * dpi_scale);
  int x = round(top_left.x * dpi_scale);
  return {x, y};
}

dimension_t Windows::win_api_layout_t::get_size(HWND &hWnd, float dpi_scale) {
  RECT rect;
  GetWindowRect(hWnd, &rect);
  int width = round((rect.right - rect.left) * dpi_scale);
  int height = round((rect.bottom - rect.top) * dpi_scale);

  return {width, height};
}

/* #endregion */
/* #region os::window */

HWND Windows::win_api_window_t::convert_gtk_to_native(gtk_window_t &window) {
  auto surface = window.get_native()->get_surface();
  if (!surface->gobj()) {
    throw gbw_error("Failed to get surface from Gtk:Window");
  }

  auto *hWnd = GDK_SURFACE_HWND(GDK_SURFACE(surface->gobj()));
  if (!hWnd) {
    throw gbw_error("Failed to get HWND from Gdk::Surface");
  }

  return hWnd;
}

float Windows::win_api_window_t::get_dpi_scale(HWND &hWnd) {
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

native_window_t Windows::win_api_window_t::top_level() {
  if (!self->top_level_window) {
    throw gbw_error("Top level native window was not set");
  }
  return self->top_level_window;
};

native_window_t Windows::win_api_window_t::browser() {
  if (!self->browser_window) {
    throw gbw_error("Browser native window was not set");
  }
  return self->browser_window;
};

/* #endregion */

/* #region os::os */
Windows::win_api_t::win_api_t(Windows *self) : os_api_t(self) {}

void Windows::win_api_t::set_native_windows(gtk_window_t &top_level,
                                            gtk_window_t &browser) {
  if (!top_level.gobj()) {
    throw gbw_error("No top level window");
  }
  if (!browser.gobj()) {
    throw gbw_error("No browser window");
  }
  auto top_level_window = window.convert_gtk_to_native(top_level);
  auto browser_window = window.convert_gtk_to_native(browser);
  self->set_native_windows(top_level_window, browser_window);
  signals.native_windows_ready().emit();
}

/* #endregion */

void Windows::attach_win32_console() {
  if (is_console_attached) {
    return;
  }
  is_console_attached = true;
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    printf("The Windows console has been attached to the parent process.\n");
  }
}
bool Windows::is_console_attached{};
