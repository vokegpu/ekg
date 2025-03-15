#include "ekg/layout/dimension.hpp"
#include "ekg/core/runtime.hpp"

float ekg::layout::estimate_docknizable_height(
  ekg::ui::abstract *p_parent_widget
) {
  if (
      p_parent_widget == nullptr
      ||
      !p_parent_widget->properties.is_docknizable
    ) {
    return 0.0f;
  }

  ekg::theme_t &current_global_theme {ekg::p_core->service_theme.get_current_theme()};
  ekg::ui::abstract *p_widgets {};
  ekg::flags_t flags {};

  float total_height {};
  float height {};

  for (ekg::properties_t *&p_properties : p_parent_widget->properties.children) {
    if (
        p_properties == nullptr
        ||
        p_properties->p_widget == nullptr
      ) {
      continue;
    }

    p_widgets = static_cast<ekg::ui::abstract*>(p_properties->p_widget);
    if (p_widgets->p_descriptor_rect == nullptr) {
      continue;
    }

    p_widgets->on_reload();

    flags = p_widgets->properties.dock;
    height = p_widgets->p_descriptor_rect->h;

    if (
        p_widgets->properties.is_docknizable
        &&
        p_widgets->properties.children.empty()
      ) {
      height = ekg::layout::estimate_docknizable_height(p_widgets); 
    }

    total_height += (
      height
      *
      (
        ekg::fequalsf(total_height, 0.0f)
        ||
        ekg::has(flags, ekg::dock::next)
      )
      +
      current_global_theme.layout_offset
    );
  }

  total_height += (
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // bottom
    +
    current_global_theme.layout_offset // bottom
  );

  return total_height;
}
