#include "os/Os.hh"
#include "os/Os_base.hh"


using namespace gbw;
using namespace gbw::os;

Os_base::~Os_base() {}

Os_base::os_api_layout_t::os_api_layout_t(Os *self) : nested_api_t(self) {};

Os_base::os_api_window_t::os_api_window_t(Os *self) : nested_api_t(self) {};

Os_base::os_api_signals_t::os_api_signals_t(Os_base *self)
    : nested_api_t(self) {}

ready_signal_t &Os_base::os_api_signals_t::native_windows_ready() {
  return self->native_windows_ready_signal;
};

Os_base::os_api_t::os_api_t(Os *self)
    : nested_api_t(self), window(self->api_window), layout(self->api_layout),
      signals(self) {};

void Os_base::set_native_windows(native_window_t &top_level,
                                 native_window_t &browser) {
  top_level_window = top_level;
  browser_window = browser;
}
