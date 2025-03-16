#include "ekg/io/algorithm.hpp"
#include "ekg/io/log.hpp"

#include <algorithm>

ekg::flags_t ekg::add_child_to_parent(
  ekg::properties_t *p_parent,
  ekg::properties_t *p_child
) {
  if (p_child == nullptr || p_parent == nullptr) {
    ekg::log() << "Failed to add child to parent, `null`, `null`";
    return ekg::result::failed;
  }

  if (
    p_child->p_parent != nullptr
    &&
    p_child->p_parent->unique_id == p_parent->unique_id
  ) {
    return ekg::result::success;
  }

  p_child->p_parent = p_parent;
  p_child->p_abs_parent = p_parent->p_abs_parent;
  p_parent->children.push_back(p_child);

  if (p_parent->is_docknizable) {
    ekg::ui::abstract *p_child_widget {
      static_cast<ekg::ui::abstract*>(p_child->p_widget)
    };

    ekg::ui::abstract *p_parent_widget {
      static_cast<ekg::ui::abstract*>(p_parent->p_widget)
    };

    p_child_widget->p_parent_rect = &p_parent_widget->properties.rect;
    p_child_widget->p_parent_scissor_rect = &p_parent_widget->scissor;
  }

  return ekg::result::success;
}

ekg::properties_t *ekg::find(
  ekg::stack_t *p_stack,
  std::string_view widget_tag
) {
  if (p_stack == nullptr || widget_tag.empty()) {
    ekg::log() << "Could not find any widget, may `` (empty) widget_tag, may `null` p_stack";
    return nullptr;
  }

  for (ekg::ui::abstract *&p_widget : p_stack->children) {
    if (p_widget != nullptr && p_widget->properties.tag == widget_tag) {
      return &p_widget->properties;
    }
  }

  return nullptr;
}

ekg::flags_t ekg::destroy(
  ekg::stack_t *p_stack,
  ekg::properties_t *p_properties
) {
  if (p_stack == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` p_stack";
    return ekg::result::failed;
  }

  if (p_properties == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` p_properties";
    return ekg::result::failed;
  }

  uint64_t counter {
    p_stack->counter++
  };

  p_properties->is_alive = false;

  if (p_properties->p_parent->is_docknizable) {
    ekg::ui::abstract *p_widget {
      static_cast<ekg::ui::abstract*>(p_properties->p_widget)
    };

    p_widget->p_parent_rect = &p_widget->_blank_parent_rect;
    p_widget->p_scroll_vec = &p_widget->_blank_scroll_vec;
  }

  for (ekg::properties_t *&p_child : p_properties->children) {
    ekg::destroy(p_stack, p_child);
  }

  if (counter == 0) {
    p_stack->counter = 0;

    if (p_properties->p_parent != nullptr) {
      std::vector<ekg::properties_t*> &parent_of_parent_children {
        p_properties->p_parent->children
      };

      parent_of_parent_children.erase(
        std::remove_if(
          parent_of_parent_children.begin(),
          parent_of_parent_children.end(),
          [p_properties](ekg::properties_t *&p_parent) {
            return p_parent->unique_id == p_properties->unique_id;
          }
        )
      );
    }
  }

  return ekg::result::success;
}

ekg::flags_t ekg::find_and_destroy(
  ekg::stack_t *p_stack,
  std::string_view widget_tag
) {
  if (p_stack == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` stack";
    return ekg::result::failed;
  }

  return ekg::destroy(p_stack, ekg::find(p_stack, widget_tag));
}

ekg::flags_t ekg::io::push_back_widget_tree_recursively(
  ekg::io::target_collector_t *p_target_collector,
  ekg::ui::abstract *p_widget
) {
  if (p_widget == nullptr || p_target_collector == nullptr) {
    ekg::log() << "Failed to push back widget tree, may `null` p_widget, may `null` p_target_collector";
    return ekg::result::failed;
  }

  if (
      p_target_collector->unique_id
      ==
      p_widget->properties.unique_id
  ) {
    p_target_collector->unique_id = p_widget->properties.unique_id;
    p_target_collector->was_target_found = true;
  }

  p_target_collector->storage.push_back(p_widget);

  for (ekg::properties_t *&p_properties : p_widget->properties.children) {
    if (
        p_properties == nullptr
        ||
        p_properties->p_widget == nullptr
      ) {
      continue;
    }

    ekg::io::push_back_widget_tree_recursively(
      p_target_collector,
      static_cast<ekg::ui::abstract*>(p_properties->p_widget)
    );
  }

  return ekg::result::success;
}
