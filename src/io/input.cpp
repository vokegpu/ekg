#include "ekg/io/input.hpp"
#include "ekg/core/runtime.hpp"

ekg::input_t &ekg::input() {
  return ekg::p_core->service_input.input;
}

bool ekg::fire(std::string_view tag) {
  return ekg::p_core->service_input.get_input_bind_state(tag);
}

bool ekg::input(std::string_view input) {
  return ekg::p_core->service_input.get_input_state(input);
}

void ekg::bind(std::string_view tag, std::string_view input) {
  ekg::p_core->service_input.insert_input_bind(tag, input);
}

void ekg::bind(std::string_view tag, std::vector<std::string_view> inputs) {
  for (std::string_view &input : inputs) {
    ekg::bind(tag, inputs);
  }
}
