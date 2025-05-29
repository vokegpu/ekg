#include "ekg/handler/callback.hpp"

ekg::callback_t ekg::callback_t::not_found {
  .at = {
    .unique_id = ekg::not_found
  }
};
