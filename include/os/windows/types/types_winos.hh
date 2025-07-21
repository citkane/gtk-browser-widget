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

#ifndef EBW_OS_WINDOWS_TYPES_TYPES_WINOS_HH
#define EBW_OS_WINDOWS_TYPES_TYPES_WINOS_HH
#ifdef _WIN32

#include <Unknwn.h>
#include <gdk/win32/gdkwin32.h>

namespace gbw::os::windows {
using native_window_t = HWND;
/// A replacement for the MSVC only wil::com_ptr smart pointer
/// @see https://github.com/microsoft/wil/wiki/WinRT-and-COM-wrappers
template <typename T> class smart_ptr {
public:
  smart_ptr() noexcept : ptr(nullptr) {}
  smart_ptr(T *p) noexcept : ptr(p) { InternalAddRef(); }
  smart_ptr(const smart_ptr &other) noexcept : ptr(other.ptr) {
    InternalAddRef();
  }
  smart_ptr(smart_ptr &&other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
  }
  ~smart_ptr() { InternalRelease(); }

  smart_ptr &operator=(const smart_ptr &other) noexcept {
    if (this != &other) {
      InternalRelease();
      ptr = other.ptr;
      InternalAddRef();
    }
    return *this;
  }
  smart_ptr &operator=(smart_ptr &other) noexcept {
    if (this != &other) {
      InternalRelease();
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }

  T *operator->() const noexcept { return ptr; }
  T **operator&() noexcept { return &ptr; }
  T *Get() const noexcept { return ptr; }
  void Reset() noexcept {
    InternalRelease();
    ptr = nullptr;
  }

private:
  void InternalAddRef() {
    if (ptr)
      ptr->AddRef();
  }
  void InternalRelease() {
    if (ptr) {
      ptr->Release();
      ptr = nullptr;
    }
  }
  T *ptr;
};

/// A replacement for the MSVC only `Microsoft::WRL::Callback` callback
/// function
/// @see
/// https://learn.microsoft.com/en-us/cpp/cppcx/wrl/callback-function-wrl?view=msvc-170
template <typename T> class Callback_handler : public T {
  ULONG refCount = 1;

public:
  virtual ~Callback_handler() = default;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
                                           void **ppvObject) override {
    if (riid == __uuidof(IUnknown) || riid == __uuidof(T)) {
      *ppvObject = static_cast<T *>(this);
      AddRef();
      return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
  }
  ULONG STDMETHODCALLTYPE AddRef() override { return ++refCount; }
  ULONG STDMETHODCALLTYPE Release() override {
    ULONG res = --refCount;
    if (res == 0)
      delete this;
    return res;
  }
};

} // namespace gbw::os::windows

#endif // _WIN32
#endif // EBW_OS_WINDOWS_TYPES_TYPES_WINOS_HH
