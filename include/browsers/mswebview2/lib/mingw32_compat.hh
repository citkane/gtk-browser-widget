#ifndef EBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH
#define EBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH
#if defined(__MINGW32__) && defined(EBW_MSWEBVIEW2)

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

#endif // defined(__MINGW32__) && defined(EBW_MSWEBVIEW2)
#endif // EBW_BROWSERS_MSWEBVIEW2_LIB_MING32_COMPAT_HH