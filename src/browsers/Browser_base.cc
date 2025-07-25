#include "browsers/Browser_base.hh"
#include "browsers/Browser.hh" // IWYU pragma: keep

using namespace gbw;
using namespace gbw::browsers;

Browser_base::~Browser_base() {}

/* #region Browser_base::api_t */

Browser_base::api_t::api_t(Browser *self)
    : nested_api_t(self), layout(self->layout_api), signals(self), api(self) {};

/* #endregion */
/* #region Browser_base::api_layout_t */

Browser_base::api_layout_t::api_layout_t(Browser *self) : nested_api_t(self) {}

/* #endregion */
/* #region Browser_base::api_signals_t */

Browser_base::api_signals_t::api_signals_t(Browser_base *self)
    : nested_api_t(self) {}

ready_signal_t &Browser_base::api_signals_t::core_ready() {
  return self->core_ready_signal;
};

ready_signal_t &Browser_base::api_signals_t::env_ready() {
  return self->environment_ready_signal;
};

ready_signal_t &Browser_base::api_signals_t::controller_ready() {
  return self->controller_ready_signal;
};

/* #endregion */
/* #region Browser_base::api_api_t */

Browser_base::api_api_t::api_api_t(Browser_base *self) : nested_api_t(self) {}

smart_core_t &Browser_base::api_api_t::core() {
  if (!self->api_core.get()) {
    throw gbw_error("Browser core API was not set");
  }
  return self->api_core;
};

smart_control_t &Browser_base::api_api_t::controller() {
  if (!self->api_controller.get()) {
    throw gbw_error("Browser controller API was not set");
  }
  return self->api_controller;
};

smart_env_t &Browser_base::api_api_t::environment() {
  if (!self->api_environment.get()) {
    throw gbw_error("Browser environment API was not set");
  }
  return self->api_environment;
};

bool Browser_base::api_api_t::ready() { return self->api_is_ready; };

/* #endregion */
/* #region Browser_base mutators */

void Browser_base::set_api_environment(browser_env_t &env) {
  std::cout << "- Got browser environment API\n";
  api_environment = smart_env_t(&env);
  environment_ready_signal.emit();
}
void Browser_base::set_api_controller(browser_controller_t &controller) {
  std::cout << "- Got browser controller API\n";
  api_controller = smart_control_t(&controller);
  controller_ready_signal.emit();
}
void Browser_base::set_api_core(browser_core_t &core) {
  std::cout << "- Got browser core API\n";
  api_core = smart_core_t(&core);
  api_is_ready = true;
  core_ready_signal.emit();
}

/* #endregion */
