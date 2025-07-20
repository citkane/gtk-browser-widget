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

#ifndef EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH
#define EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH
#if defined(_WIN32) && defined(EBW_MSWEBVIEW2)

#include "browsers/mswebview2/lib/mingw32_compat.hh"  // IWYU pragma: keep
#include <WebView2.h>

namespace gbw::browsers::mswebview2 {

#define create_webview2 CreateCoreWebView2EnvironmentWithOptions
// #define create_controller
// ICoreWebView2Environment::CreateCoreWebView2Controller

using browser_env_t = ICoreWebView2Environment;
using browser_controller_t = ICoreWebView2Controller;
using browser_core_t = ICoreWebView2;

} // namespace gbw::browsers::mswebview2

#endif // defined(_WIN32) && defined(EBW_MSWEBVIEW2)
#endif // EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH
