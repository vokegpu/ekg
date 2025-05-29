#ifndef EKG_IO_DESCRIPTOR_HPP
#define EKG_IO_DESCRIPTOR_HPP

#include "ekg/io/memory.hpp"

namespace ekg {
  enum class type {
    callback,
    property,
    stack,
    button
  };
}

#define EKG_DESCRIPTOR(descriptor_t) \
  public: \
    ekg::at_t at {}; \
  public: \
    bool operator == (descriptor_t &descriptor) { \
      descriptor_t::not_found.unique_id = ekg::not_found; \
      return this->at.unique_id == descriptor.at.unique_id; \
    }
\
    bool operator != (descriptor_t &descriptor) { \
      return !(*this == descriptor); \
    } \
\
    operator ekg::at_t() { \
      return this->at; \
    }

#endif
