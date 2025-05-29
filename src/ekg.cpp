#include "ekg/ekg.hpp"
#include "ekg/io/log.hpp"

ekg::runtime_t ekg::p_core {nullptr};
std::ostringstream ekg::log::buffer {};
bool ekg::log::buffered {};

ekg::flags_t ekg::init(
  ekg::ekg_runtime_properties_info_t &runtime_properties_info,
  ekg::runtime_t *p_runtime
) {
  if (p_runtime == nullptr) {
    ekg::log("[EKG] ~ERROR~ invalid (?) `ekg::runtime_t` pointer address: nullptr");
    return ekg::result::failed;
  }

  ekg::p_core = p_runtime;
  return ekg::result::success;
}

void ekg::quit() {
  if (ekg::p_core == nullptr) {
    return;
  }
}

void ekg::update() {
  if (ekg::p_core == nullptr) {
    return;
  }
}

void ekg::render() {
  if (ekg::p_core == nullptr) {
    return;
  }
}
