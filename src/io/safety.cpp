#include "ekg/io/safety.hpp"

void ekg::pop(ekg::properties_t *p_properties) {
  ekg::p_core->set_current_parent_properties(
    p_properties
  );
}
