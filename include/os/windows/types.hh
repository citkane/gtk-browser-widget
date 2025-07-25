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

#ifndef GBW_OS_WINDOWS_TYPES_HH
#define GBW_OS_WINDOWS_TYPES_HH
#ifdef _WIN32

#include "os/windows/lib/smart_templates.hh"

namespace gbw {
namespace os {
class Windows;

namespace types {

using native_window_t = HWND;

template <typename T> class smart_ptr : public windows::lib::smart_ptr<T> {
public:
  using windows::lib::smart_ptr<T>::smart_ptr;
  using windows::lib::smart_ptr<T>::operator=;
  using windows::lib::smart_ptr<T>::operator->;
  using windows::lib::smart_ptr<T>::operator&;

  T *get() const noexcept { return this->Get(); }
};

} // namespace types
} // namespace os
using Os = gbw::os::Windows;

} // namespace gbw

#endif // _WIN32
#endif // GBW_OS_WINDOWS_TYPES_HH
