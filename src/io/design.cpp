#include "ekg/io/design.hpp"
#include "ekg/core/runtime.hpp"

std::map<std::string_view, ekg::theme_t> &ekg::themes() {
  return ekg::p_core->service_theme.get_theme_map();
}

ekg::theme_t &ekg::theme(std::string_view name) {
  if (name.empty()) {
    return ekg::p_core->service_theme.get_current_theme();
  }

  return ekg::themes()[name];
}

void ekg::theme(ekg::theme_t theme) {
  return ekg::p_core->service_theme.add(theme);
}

ekg::flags_t ekg::set_current_theme(std::string_view name) {
  return ekg::p_core->service_theme.set_current_theme(name);
}
