#include "gtk/lib/Lib_gtk.hh"

using namespace gbw;
using namespace gbw::gtk;

bool Lib_gtk::is_using_gtk_csd(gtk_window_t &window) {
  auto is_application = !!window.get_application();
  auto has_titlebar = !!window.get_titlebar();
  auto csd_env = std::getenv("GTK_CSD");
  auto csd_env_disabled = !!csd_env && std::string(csd_env) == "0";
  auto undecorated = !window.get_decorated();

  // std::cout << "is_application: " << (is_application ? "true" : "false")
  //           << "\n";
  // std::cout << "has_titlebar: " << (has_titlebar ? "true" : "false") << "\n";
  // std::cout << "csd_env_disabled: " << (csd_env_disabled ? "true" : "false")
  //           << "\n";
  // std::cout << "undecorated: " << (undecorated ? "true" : "false") << "\n";

  if (undecorated || (csd_env_disabled && !has_titlebar)) {
    return false;
  }
  return true;
  // return has_titlebar;
}

void Lib_gtk::set_csd_fudge(int x, int y) { csd_fudge = {x, y}; };
offset_t &Lib_gtk::get_csd_fudge() { return csd_fudge; }
offset_t Lib_gtk::csd_fudge{0, 0};