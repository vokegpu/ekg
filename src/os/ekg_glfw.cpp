#include "ekg/os/ekg_glfw.hpp"
#include "ekg/core/context.hpp"
#include "ekg/core/runtime.hpp"

ekg::glfw::glfw(
  GLFWwindow *p_glfw_win,
  ekg::flags_t modes
) {
  this->p_glfw_win = p_glfw_win;
  this->modes = modes;
  this->update_display_size();

  int32_t w {}, h {};

  /**
   * Sounds an unnecessary resize, but it is invoke a complete swapchain redo,
   * to fix the orthographic matrix neededs calc.
   */

  glfwGetWindowSize(this->p_glfw_win, &w, &h);
  glfwSetWindowSize(this->p_glfw_win, w--, h--);
  glfwSetWindowSize(this->p_glfw_win, w++, h++);
}

void ekg::glfw::init() {
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::arrow)]      = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::ibeam)]      = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::wait)]       = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::crosshair)]  = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::wait_arrow)] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_nwse)]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_nesw)]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_we)]    = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_ns)]    = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_all)]   = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::no)]         = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::hand)]       = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

void ekg::glfw::quit() {

}

void ekg::glfw::update_display_size() {
  const GLFWvidmode *p_glfw_vidmode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

  this->display_size.x = p_glfw_vidmode->width;
  this->display_size.y = p_glfw_vidmode->height;
}

void ekg::glfw::update() {
  ekg::timing_t::ticks = static_cast<uint64_t>(glfwGetTime() * 1000.0f);

  glfwSetCursor(
    this->p_glfw_win,
    this->loaded_system_cursor_list[static_cast<uint64_t>(system_cursor)]
  );
}

void ekg::glfw::get_key_name(ekg::io::input_key_t &key, std::string &name) {
  switch (key.key) {
    case GLFW_KEY_LEFT_CONTROL:
      name = "lctrl";
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      name = "rctrl";
      break;
    case GLFW_KEY_LEFT_SHIFT:
      name = "lshift";
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      name = "rshift";
      break;
    case GLFW_KEY_LEFT_ALT:
      name = "alt";
      break;
    case GLFW_KEY_RIGHT_ALT:
      name = "altgr";
      break;
    case GLFW_KEY_TAB:
      name = "tab";
      break;
    case GLFW_KEY_ESCAPE:
      name = "ESCAPE";
      break;
    case GLFW_KEY_ENTER:
      name = "return";
      break;
    case GLFW_KEY_BACKSPACE:
      name = "BACKSPACE";
      break;
    case GLFW_KEY_INSERT:
      name = "INSERT";
      break;
    case GLFW_KEY_DELETE:
      name = "DELETE";
      break;
    case GLFW_KEY_RIGHT:
      name = "RIGHT";
      break;
    case GLFW_KEY_LEFT:
      name = "LEFT";
      break;
    case GLFW_KEY_DOWN:
      name = "DOWN";
      break;
    case GLFW_KEY_UP:
      name = "UP";
      break;
    case GLFW_KEY_PAGE_UP:
      name = "PAGE_UP";
      break;
    case GLFW_KEY_PAGE_DOWN:
      name = "PAGE_DOWN";
      break;
    case GLFW_KEY_HOME:
      name = "HOME";
      break;
    case GLFW_KEY_END:
      name = "END";
      break;
    case GLFW_KEY_CAPS_LOCK:
      name = "CAPS_LOCK";
      break;
    case GLFW_KEY_SCROLL_LOCK:
      name = "SCROLL_LOCK";
      break;
    case GLFW_KEY_NUM_LOCK:
      name = "NUM_LOCK";
      break;
    case GLFW_KEY_PRINT_SCREEN:
      name = "PRINT_SCREEN";
      break;
    case GLFW_KEY_PAUSE:
      name = "PAUSE";
      break;
    case GLFW_KEY_F1:
      name = "F1";
      break;
    case GLFW_KEY_F2:
      name = "F2";
      break;
    case GLFW_KEY_F3:
      name = "F3";
      break;
    case GLFW_KEY_F4:
      name = "F4";
      break;
    case GLFW_KEY_F5:
      name = "F5";
      break;
    case GLFW_KEY_F6:
      name = "F6";
      break;
    case GLFW_KEY_F7:
      name = "F7";
      break;
    case GLFW_KEY_F8:
      name = "F8";
      break;
    case GLFW_KEY_F9:
      name = "F9";
      break;
    case GLFW_KEY_F10:
      name = "F10";
      break;
    case GLFW_KEY_F11:
      name = "F11";
      break;
    case GLFW_KEY_F12:
      name = "F12";
      break;
    case GLFW_KEY_F13:
      name = "F13";
      break;
    case GLFW_KEY_F14:
      name = "F14";
      break;
    case GLFW_KEY_F15:
      name = "F15";
      break;
    case GLFW_KEY_F16:
      name = "F16";
      break;
    case GLFW_KEY_F17:
      name = "F17";
      break;
    case GLFW_KEY_F18:
      name = "F18";
      break;
    case GLFW_KEY_F19:
      name = "F19";
      break;
    case GLFW_KEY_F20:
      name = "F20";
      break;
    case GLFW_KEY_F21:
      name = "F21";
      break;
    case GLFW_KEY_F22:
      name = "F22";
      break;
    case GLFW_KEY_F23:
      name = "F23";
      break;
    case GLFW_KEY_F24:
      name = "F24";
      break;
    case GLFW_KEY_F25:
      name = "F25";
      break;
    case GLFW_KEY_KP_0:
      name = "KP_0";
      break;
    case GLFW_KEY_KP_1:
      name = "KP_1";
      break;
    case GLFW_KEY_KP_2:
      name = "KP_2";
      break;
    case GLFW_KEY_KP_3:
      name = "KP_3";
      break;
    case GLFW_KEY_KP_4:
      name = "KP_4";
      break;
    case GLFW_KEY_KP_5:
      name = "KP_5";
      break;
    case GLFW_KEY_KP_6:
      name = "KP_6";
      break;
    case GLFW_KEY_KP_7:
      name = "KP_7";
      break;
    case GLFW_KEY_KP_8:
      name = "KP_8";
      break;
    case GLFW_KEY_KP_9:
      name = "KP_9";
      break;
    case GLFW_KEY_KP_DECIMAL:
      name = "KP_DECIMAL";
      break;
    case GLFW_KEY_KP_DIVIDE:
      name = "KP_DIVIDE";
      break;
    case GLFW_KEY_KP_MULTIPLY:
      name = "KP_MULTIPLY";
      break;
    case GLFW_KEY_KP_SUBTRACT:
      name = "KP_SUBTRACT";
      break;
    case GLFW_KEY_KP_ADD:
      name = "KP_ADD";
      break;
    case GLFW_KEY_KP_ENTER:
      name = "return";
      break;
    case GLFW_KEY_KP_EQUAL:
      name = "KP_EQUAL";
      break;
    case GLFW_KEY_LEFT_SUPER:
      name = "LEFT_SUPER";
      break;
    case GLFW_KEY_RIGHT_SUPER:
      name = "RIGHT_SUPER";
      break;
    case GLFW_KEY_MENU:
      name = "MENU";
      break;
    default:
      const char *p_key_name {glfwGetKeyName(key.key, key.scancode)};
      if (p_key_name) {
        name = glfwGetKeyName(key.key, key.scancode);
      }
      break;
  }
}

void ekg::glfw::get_special_key(ekg::io::input_key_t &key, ekg::special_key_type &special_key) {
  switch (key.key) {
    case GLFW_KEY_LEFT_CONTROL:
      special_key = ekg::special_key_type::left_ctrl;
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      special_key = ekg::special_key_type::right_ctrl;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      special_key = ekg::special_key_type::left_shift;
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      special_key = ekg::special_key_type::right_shift;
      break;
    case GLFW_KEY_LEFT_ALT:
      special_key = ekg::special_key_type::left_alt;
      break;
    case GLFW_KEY_RIGHT_ALT:
      special_key = ekg::special_key_type::right_alt;
      break;
    case GLFW_KEY_TAB:
      special_key = ekg::special_key_type::tab;
      break;
    default:
      special_key = ekg::special_key_type::unknown;
      break;
  }
}

const char *ekg::glfw::get_clipboard_text() {
  return glfwGetClipboardString(this->p_glfw_win);
}

void ekg::glfw::set_clipboard_text(const char *p_text) {
  glfwSetClipboardString(this->p_glfw_win, p_text);

}

bool ekg::glfw::has_clipboard_text() {
  return glfwGetClipboardString(this->p_glfw_win) != NULL;
}

void ekg::glfw_window_size_callback(int32_t w, int32_t h) {
  if (
      ekg::has(
        ekg::p_core->p_os_platform->modes,
        ekg::internal_behavior::no_auto_set_viewport_when_resize
      )
    ) {
    return;
  }

  ekg::viewport.w = static_cast<float>(w);
  ekg::viewport.h = static_cast<float>(h);

  ekg::p_core->p_gpu_api->update_viewport(ekg::viewport.w, ekg::viewport.h);
  ekg::io::dispatch(ekg::io::operation::scale_update);
}

void ekg::glfw_scroll_callback(double dx, double dy) {
  ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_wheel;
  ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_x = static_cast<int32_t>(dx);
  ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_y = static_cast<int32_t>(dy);
  ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_precise_x = dx;
  ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_precise_y = dy;

  ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
  ekg::p_core->poll_events();
}

void ekg::glfw_char_callback(uint32_t codepoint) {
  ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::text_input;

  // idk it seems pretty much a workaround, predictable crash if codepoint
  // is larger than 127 (overflow)
  const char c [1] {static_cast<char>(codepoint)};
  ekg::p_core->p_os_platform->serialized_input_event.text_input = (c);

  ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
  ekg::p_core->poll_events();
}

void ekg::glfw_key_callback(int32_t key, int32_t scancode, int32_t action, int32_t mods) {
  switch (action) {
  case GLFW_PRESS:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::key_down;
    ekg::p_core->p_os_platform->serialized_input_event.key.key = key;
    ekg::p_core->p_os_platform->serialized_input_event.key.scancode = scancode;
    break;
  
  case GLFW_REPEAT:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::key_down;
    ekg::p_core->p_os_platform->serialized_input_event.key.key = key;
    ekg::p_core->p_os_platform->serialized_input_event.key.scancode = scancode;
    break;

  case GLFW_RELEASE:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::key_up;
    ekg::p_core->p_os_platform->serialized_input_event.key.key = key;
    ekg::p_core->p_os_platform->serialized_input_event.key.scancode = scancode;
    break;
  }

  ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
  ekg::p_core->poll_events();
}

void ekg::glfw_mouse_button_callback(int32_t button, int32_t action, int32_t mods) {

  /**
   * The mouse button number on GLFW is different from SDL2,
   * but EKG is mainly SDL-based; then the `mouse_button`
   * will convert to SDL code sequences:
   * 
   * (converted) (glfw original button code)
   * 1 0
   * 2 2
   * 3 1
   * 4 3
   * 5 4
   * ... 
   **/

  switch (action) {
  case GLFW_PRESS:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_button_down;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_button = (1 + (button == 1) + button - (1 * (button == 2)));
    break;

  case GLFW_RELEASE:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_button_up;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_button = (1 + (button == 1) + button - (1 * (button == 2)));
    break;
  }

  ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
  ekg::p_core->poll_events();
}

void ekg::glfw_cursor_pos_callback(double x, double y) {
  ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_motion;
  ekg::p_core->p_os_platform->serialized_input_event.mouse_motion_x = static_cast<float>(x);
  ekg::p_core->p_os_platform->serialized_input_event.mouse_motion_y = static_cast<float>(y);

  ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
  ekg::p_core->poll_events();
}
