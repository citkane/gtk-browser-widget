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

#ifndef GBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH
#define GBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH
#if defined(__MINGW32__) && defined(GBW_MSWEBVIEW2)

#include <WebView2.h>
#include <_mingw.h>
#include <initguid.h>

__CRT_UUID_DECL(ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler,
                0x4E8A3389, 0xC9D8, 0x4BD2, 0xB6, 0xB2, 0x09, 0x6E, 0xB7, 0xD2,
                0xF2, 0xD2)
__CRT_UUID_DECL(ICoreWebView2CreateCoreWebView2ControllerCompletedHandler,
                0x6C4819F3, 0xC9B7, 0x4260, 0x81, 0x27, 0xD7, 0x13, 0x86, 0x9A,
                0xC0, 0xEB)
// Add similar lines for any other interfaces you use

#endif // defined(__MINGW32__) && defined(GBW_MSWEBVIEW2)
#endif // GBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH
