#ifndef EBW_TYPES_INCLUDES_HH
#define EBW_TYPES_INCLUDES_HH

#ifdef __WIN32__
#include "os/windows/types/types_winos.hh" // IWYU pragma: keep
using namespace gbw::os::windows;

#ifdef EBW_MSWEBVIEW2
#include "browsers/mswebview2/types/types_wv2.hh" // IWYU pragma: keep
using namespace gbw::browsers::mswebview2;

#endif // EBW_MSWEBVIEW2
#endif // __WIN32__
#endif // EBW_TYPES_INCLUDES_HH