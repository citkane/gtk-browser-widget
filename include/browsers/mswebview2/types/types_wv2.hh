#ifndef EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH
#define EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH
#if defined(__WIN32__) && defined(EBW_MSWEBVIEW2)

#include "browsers/mswebview2/lib/mingw32_compat.hh"

namespace gbw::browsers::mswebview2 {

#define create_webview2 CreateCoreWebView2EnvironmentWithOptions
// #define create_controller
// ICoreWebView2Environment::CreateCoreWebView2Controller

using browser_env_t = ICoreWebView2Environment;
using browser_controller_t = ICoreWebView2Controller;
using browser_core_t = ICoreWebView2;

} // namespace gbw::browsers::mswebview2

#endif // defined(__WIN32__) && defined(EBW_MSWEBVIEW2)
#endif // EBW_BROWSERS_MSWEBVIEW2_TYPES_TYPES_WV2_HH