#include "browsers/lib/Lib_browser.hh"
#include "browsers/lib/includes.hh"

using namespace gbw;
using namespace gbw::browsers;

Lib_browser::browser_api_layout_t::browser_api_layout_t(Browser_engine *self)
    : nested_api_t(self) {}

Lib_browser::browser_api_signals_t::browser_api_signals_t(Lib_browser *self)
    : nested_api_t(self) {}

/// The signal for the browser window ready event.
ready_signal_t &Lib_browser::browser_api_signals_t::core_ready() {
  return self->core_ready_signal;
};

/// The signal for the browser environment ready event.
ready_signal_t &Lib_browser::browser_api_signals_t::env_ready() {
  return self->environment_ready_signal;
};

/// The signal for the browser controller ready event.
ready_signal_t &Lib_browser::browser_api_signals_t::controller_ready() {
  return self->controller_ready_signal;
};

Lib_browser::browser_api_api_t::browser_api_api_t(Lib_browser *self)
    : nested_api_t(self) {}

/// Gets the browser core API (ie. the window instance DOM/JS API)
smart_core_t &Lib_browser::browser_api_api_t::core() {
  if (!self->api_core.Get()) {
    throw gbw_error("Browser core API was not set");
  }
  return self->api_core;
};

/// Gets the browser controller API (ie. the browser engine settings)
smart_control_t &Lib_browser::browser_api_api_t::controller() {
  if (!self->api_controller.Get()) {
    throw gbw_error("Browser controller API was not set");
  }
  return self->api_controller;
};

/// Gets the browser environment API (ie. the browser engine environment)
smart_env_t &Lib_browser::browser_api_api_t::environment() {
  if (!self->api_environment.Get()) {
    throw gbw_error("Browser environment API was not set");
  }
  return self->api_environment;
};

bool Lib_browser::browser_api_api_t::ready() { return self->api_is_ready; };

Lib_browser::browser_api_t::browser_api_t(Browser_engine *self)
    : nested_api_t(self), layout(self->layout_api), signals(self), api(self) {};

void Lib_browser::set_api_environment(browser_env_t &env) {
  std::cout << "- Got browser environment API\n";
  api_environment = smart_env_t(&env);
  environment_ready_signal.emit();
}
void Lib_browser::set_api_controller(browser_controller_t &controller) {
  std::cout << "- Got browser controller API\n";
  api_controller = smart_control_t(&controller);
  controller_ready_signal.emit();
}
void Lib_browser::set_api_core(browser_core_t &core) {
  std::cout << "- Got browser core API\n";
  api_core = smart_core_t(&core);
  api_is_ready = true;
  core_ready_signal.emit();
}