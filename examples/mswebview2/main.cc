// gtkmm headers MUST be included first
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/paned.h>
#include <gtkmm/popovermenubar.h>

#include "Gbw_widget.hh"

class Application : public Gtk::ApplicationWindow {
public:
  Application() : browser() {

    browser.ready().connect([this] {
      auto bw = browser.api_core();
      bw->Navigate(L"https://www.gtk.org/");
      // bw->OpenDevToolsWindow();
    });

    set_title("Example GBW application");
    set_default_size(1000, 900);

    setup_menu();
    setup_panes();

    m_paned.set_end_child(browser);
    m_vbox.append(m_menubar);
    m_vbox.append(m_paned);

    set_child(m_vbox);
  };

private:
  gbw::Gbw_widget browser;
  Gtk::PopoverMenuBar m_menubar;
  Gtk::Box m_vbox{Gtk::Orientation::VERTICAL};
  Gtk::Paned m_paned{Gtk::Orientation::HORIZONTAL};

  void setup_menu() {
    auto file_menu = Gio::Menu::create();
    file_menu->append("_Quit", "app.quit");
    auto m_menu_model = Gio::Menu::create();
    m_menu_model->append_submenu("_File", file_menu);
    m_menubar = Gtk::PopoverMenuBar();
    m_menubar.set_menu_model(m_menu_model);
  }

  void setup_panes() {
    Gtk::Label m_left_label{"Gtk resizeable pane"};
    m_paned.set_start_child(m_left_label);
    m_paned.set_resize_start_child(true);
    m_paned.set_resize_end_child(true);
    m_paned.set_vexpand(true);
    m_paned.set_position(200);
  }
};

#ifdef _WIN32
int CALLBACK WinMain(_In_ HINSTANCE /*hInstance*/,
                     _In_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/,
                     _In_ int /*nCmdShow*/) {

  gbw::options::attach_win32_console();
  gbw::options::gtk::csd::set_fudge(0, -2);
  // gbw::options::try_bypass_gtk_csd();

#else
int main() {
#endif

  auto app = Gtk::Application::create("org.gbw.example");

  auto action_quit = Gio::SimpleAction::create("quit");
  app->add_action(action_quit);
  action_quit->signal_activate().connect(
      [&app](const Glib::VariantBase &) { app->get_run_window()->close(); });

  return app->make_window_and_run<Application>(0, nullptr);
};
