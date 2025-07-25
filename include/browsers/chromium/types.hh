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

#ifndef GBW_BROWSERS_CHROMIUM_TYPES_HH
#define GBW_BROWSERS_CHROMIUM_TYPES_HH

#if defined(_WIN32) && defined(GBW_CHROMIUM)

#include "browsers/chromium/lib/mingw32_compat.hh" // IWYU pragma: keep
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"

namespace gbw {
namespace browsers {
class Chromium;

namespace types {

using browser_env_t = CefRefPtr<CefApp>;
using browser_controller_t = CefRefPtr<CefBrowser>;
using browser_core_t = CefRefPtr<CefBrowserHost>;

} // namespace types
}
using Browser = gbw::browsers::Chromium;
}
#endif
#endif
