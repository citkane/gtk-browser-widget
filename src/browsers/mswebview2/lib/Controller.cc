#include "browsers/mswebview2/lib/Controller.hh"
#include "browsers/mswebview2/Ms_Webview2.hh"

using namespace gbw::browsers::mswebview2::lib_mswebview2;

HRESULT STDMETHODCALLTYPE Controller::CompletedHandler::Invoke(
    HRESULT result, browser_controller_t *controller) {
  if (controller == nullptr) {
    throw ebw_error("Failed to Invoke CoreWebview2 controller");
  }
  webview->set_browser_controller(*controller);

  browser_core_t *core;
  if (controller->get_CoreWebView2(&core) != S_OK) {
    throw ebw_error("Failed to get ICoreWebview2 from controller");
  }
  webview->set_browser_core(*core);
  return S_OK;
}
