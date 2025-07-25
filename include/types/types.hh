/*
 * MIT License
 *
 * Copyright (c) 2025 Michael Jonker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GBW_TYPES_TYPES_HH
#define GBW_TYPES_TYPES_HH

#include <gdkmm/event.h>
#include <gdkmm/graphene_point.h>
#include <gdkmm/graphene_rect.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>
#include <optional>

#include "browsers/types.hh" // IWYU pragma: keep
#include "os/types.hh"       // IWYU pragma: keep
#include "types/errors.hh"   // IWYU pragma: keep

using namespace gbw::types;
using namespace gbw::browsers::types;
using namespace gbw::os::types;

namespace gbw {

class Gbw_widget;

namespace core {
class Gbw_window;

} // namespace core

namespace types {

template <typename T> struct nested_api_t {
  nested_api_t(T *self) : self(self) {}
  T *self;
};

struct position_t {
  int x;
  int y;
};
struct dimension_t {
  int width;
  int height;
};
struct layout_t {
  int x;
  int y;
  int width;
  int height;
};
struct layout_eq_t {
  bool origin;
  bool size;
};

constexpr Gdk::Event::Type FOCUS_CHANGE = Gdk::Event::Type::FOCUS_CHANGE;

using gdk_event_t = const Glib::RefPtr<const Gdk::Event>;
using gdk_bounds_t = std::optional<Gdk::Graphene::Rect>;
using gdk_position_t = std::optional<Gdk::Graphene::Point>;

using gtk_window_t = Gtk::Window;
using gtk_widget_t = Gtk::Widget;

using gbw_browser_t = gbw::core::Gbw_window;
using gbw_widget_t = gbw::Gbw_widget;

using ready_signal_t = sigc::signal<void()>;

using smart_env_t = smart_ptr<browser_env_t>;
using smart_control_t = smart_ptr<browser_controller_t>;
using smart_core_t = smart_ptr<browser_core_t>;

} // namespace types
} // namespace gbw

#endif // GBW_TYPES_TYPES_HH
