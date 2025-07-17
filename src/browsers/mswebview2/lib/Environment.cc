#include "browsers/mswebview2/lib/Environment.hh"
#include "browsers/mswebview2/Ms_Webview2.hh"

using namespace gbw::browsers::mswebview2::lib_mswebview2;

HRESULT STDMETHODCALLTYPE
Environment::CompletedHandler::Invoke(HRESULT result, browser_env_t *env) {
  if (!env) {
    throw ebw_error("Failed to invoke MsWebview2 environment");
  }

  webview->set_browser_env(*env);
  return S_OK;
}
