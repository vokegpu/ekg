#include "ekg/core/runtime.hpp"
#include "ekg/layout/scale.hpp"
#include "ekg/core/context.hpp"
#include "ekg/draw/shape.hpp"

ekg::runtime *ekg::p_core {};

void ekg::runtime::init() {
  this->service_handler.init();

  /**
   * Allocate order is important here, all pre-allocated tasks MUST follow
   * the `ekg::io::operation` order.
   * 
   * Example:
   * ```c++
   * enum operation {
   *   swap, // first
   *   // etc
   *   // etc
   *   redraw // MUST BE the last one
   * };
   * ```
   **/

  this->swap_target_collector.unique_id = ekg::io::invalid_unique_id;
  this->service_handler.allocate() = new ekg::task_t {
    .info = ekg::info_t {
      .tag = "swap",
      .p_properties = nullptr,
      .p_data = nullptr
    },
    .function = [this](ekg::info_t &info) {
      if (this->swap_target_collector.unique_id == ekg::io::invalid_unique_id) {
        return;
      }

      ekg::flags_t result {};
      std::vector<ekg::ui::abstract*> top_level_widget_list {};
      this->context_widget_list.clear();

      for (std::unique_ptr<ekg::ui::abstract> &p_widget : this->loaded_widget_list) {
        if (
            p_widget->properties.p_abs_parent == nullptr
            ||
            /**
             * TODO: check this right
             **/
            p_widget->properties.p_abs_parent->p_widget == nullptr
          ) {
          continue;
        }
 
        this->swap_target_collector.was_target_found = false;
        this->swap_target_collector.storage.clear();

        result = ekg::io::push_back_widget_tree_recursively(
          &this->swap_target_collector,
          static_cast<ekg::ui::abstract*>(p_widget->properties.p_abs_parent->p_widget)
        );

        if (result != ekg::result::success) {
          continue;
        }

        if (this->swap_target_collector.was_target_found) {
          top_level_widget_list.insert(
            top_level_widget_list.begin(),
            this->swap_target_collector.storage.begin(),
            this->swap_target_collector.storage.end()
          );
        } else {
          this->context_widget_list.insert(
            this->context_widget_list.end(),
            this->swap_target_collector.storage.begin(),
            this->swap_target_collector.storage.end()
          );
        }
      }

      this->context_widget_list.insert(
        this->context_widget_list.end(),
        top_level_widget_list.begin(),
        top_level_widget_list.end()
      );

      this->swap_target_collector.storage.clear();
      this->swap_target_collector.unique_id = ekg::io::invalid_unique_id;
    }
  };

  this->service_handler.allocate() = new ekg::task_t {
    .info = {
      .tag = "reload",
      .p_properties = nullptr,
      .p_data = nullptr
    },
    .function = [this](ekg::info_t &info) {
      for (ekg::ui::abstract *&p_widgets : this->reload_widget_list) {
        if (!p_widgets->states.was_reloaded) {
          continue;
        }

        p_widgets->on_reload();
        p_widgets->states.was_reloaded = false;
      }

      this->reload_widget_list.clear();
      ekg::viewport.redraw = true;
    }
  };

  this->service_handler.allocate() = new ekg::task_t {
    .info = {
      .tag = "layout-docknize",
      .p_properties = nullptr,
      .p_data = nullptr
    },
    .function = [this](ekg::info_t &info) {
      for (ekg::ui::abstract *&p_widgets : this->layout_docknize_list) {
        if (!p_widgets->states.was_layout_docknized) {
          continue;
        }

        ekg::layout::docknize_widget(p_widgets);
        p_widgets->states.was_layout_docknized = false;
      }

      this->layout_docknize_list.clear();
    }
  };

  this->service_handler.allocate() = new ekg::task_t {
    .info = {
      .tag = "scale-update",
      .p_properties = nullptr,
      .p_data = nullptr
    },
    .function = [this](ekg::info_t &info) {
      ekg::layout::scale_calculate();

      ekg::dpi.font_scale = ekg::clamp<float>(
        ekg::dpi.font_scale,
        static_cast<float>(ekg::minimum_font_height),
        static_cast<float>(UINT8_MAX)
      );

      uint32_t font_size {
        ekg::clamp<uint32_t>(
          static_cast<uint32_t>(
            ekg::dpi.font_scale
            *
            ekg::dpi.factor_scale
          ),
          0,
          UINT8_MAX
        )
      };

      if (this->draw_fr_normal.font_size != font_size) {
        this->draw_fr_small.set_size(
          ekg::clamp_min(
            font_size - ekg::dpi.font_offset.x,
            ekg::minimum_small_font_height
          )
        );

        this->draw_fr_normal.set_size(
          ekg::clamp_min(
            font_size,
            ekg::minimum_font_height
          )
        );

        this->draw_fr_big.set_size(
          ekg::clamp_min(
            font_size + ekg::dpi.font_offset.y,
            ekg::minimum_big_font_height
          )
        );
      }

      for (ekg::ui::abstract *&p_widgets : this->context_widget_list) {
        if (p_widgets == nullptr || !p_widgets->properties.is_docknizable) {
          continue;
        }

        this->layout_docknize_list.push_back(p_widgets);
        p_widgets->states.was_layout_docknized = true;
        p_widgets->properties.must_refresh_size = true;
      }

      ekg::io::dispatch(
        ekg::io::operation::reload
      );

      ekg::io::dispatch(
        ekg::io::operation::layout_docknize
      );
    }
  };

  this->gpu_allocator.init();
  this->service_theme.init();
  this->service_input.init();

  ekg::log() << "Doing font-rendering tweaks, and pre-setting viewport scale...";

  this->draw_fr_small.atlas_texture_sampler.gl_protected_active_index = true;
  this->draw_fr_small.set_size(16);
  this->draw_fr_small.bind_allocator(&this->gpu_allocator);

  this->draw_fr_normal.atlas_texture_sampler.gl_protected_active_index = true;
  this->draw_fr_normal.set_size(18);
  this->draw_fr_normal.bind_allocator(&this->gpu_allocator);

  this->draw_fr_big.atlas_texture_sampler.gl_protected_active_index = true;
  this->draw_fr_big.set_size(24);
  this->draw_fr_big.bind_allocator(&this->gpu_allocator);
}

void ekg::runtime::quit() {
  this->service_handler.quit();
  this->service_theme.quit();
  this->service_input.quit();
  this->gpu_allocator.quit();
}

void ekg::runtime::poll_events() {
  this->service_input.on_event();

  ekg::input_t &input {
    this->service_input.input
  };

  bool is_on_scrolling_timeout {!ekg::reach(&input.ui_scrolling_timing, 100)};
  ekg::current.unique_id *= !(input.was_pressed || input.was_released || input.has_motion);

  if (
      this->p_abs_activity_widget != nullptr
      &&
      (this->p_abs_activity_widget->states.is_absolute || is_on_scrolling_timeout)
    ) {

    this->p_abs_activity_widget->on_event(ekg::io::stage::pre);
    this->p_abs_activity_widget->on_event(ekg::io::stage::process);

    if (
        this->p_abs_activity_widget->states.is_scrolling.x
        ||
        this->p_abs_activity_widget->states.is_scrolling.y
      ) {
      ekg::reset(&input.ui_scrolling_timing);
    }

    this->p_abs_activity_widget->on_event(ekg::io::stage::post);
    return;
  }

  if (is_on_scrolling_timeout) {
    return;
  }

  this->p_abs_activity_widget = nullptr;

  bool hovered {};
  bool first_absolute {};

  ekg::ui::abstract *p_widget_focused {};
  for (ekg::ui::abstract *&p_widgets : this->context_widget_list) {
    if (p_widgets == nullptr || !p_widgets->properties.is_alive) {
      continue;
    }

    p_widgets->on_event(ekg::io::stage::pre);

    /**
     * Text input like textbox and keyboard events should not update stack, only mouse events.
     **/
    hovered = (
      !(
        this->p_os_platform->serialized_input_event.type == ekg::io::input_event_type::key_down
        ||
        this->p_os_platform->serialized_input_event.type == ekg::io::input_event_type::key_up
        ||
        this->p_os_platform->serialized_input_event.type == ekg::io::input_event_type::text_input
      )
      &&
      p_widgets->states.is_hovering
      &&
      p_widgets->properties.is_visible
      &&
      p_widgets->properties.is_enabled
    );

    if (hovered) {
      p_widget_focused && (p_widget_focused->states.is_hovering = false);
      p_widget_focused = p_widgets;
      first_absolute = false;
    }

    /**
     * The absolute/top-level system check for the first absolute fired widget,
     * everytime a widget is hovered then reset again the checking state.
     *
     * The order of scrollable widgets like scroll widget is not sequentially,
     * e.g, the mouse is hovering some children of frame 2 and absolute widget scroll from frame 2 is fired:
     *
     * frame 1           // hovered, check for the first absolute
     *
     * frame 2 (frame 1) // hovered, then reset and find for the first absolute again
     * widgets...        // hovering some of children widgets, then reset over again
     * scroll (frame 2)  // found the first absolute, target it
     *
     * frame 3 (frame 1) // not hovering, then does not reset any absolute checking
     * ...
     *
     * scroll (frame 1)  // do not target this fired absolute widget.
     * end of e.g.
     **/
    if (p_widgets->states.is_absolute && !first_absolute) {
      p_widget_focused = p_widgets;
      first_absolute = true;
    }

    p_widgets->on_event(ekg::io::stage::post);
    if (!hovered && !p_widgets->states.is_absolute) {
      p_widgets->states.is_hovering = false;
      p_widgets->on_event(ekg::io::stage::process);
    }
  }

  ekg::current.type = ekg::type::abstract;

  if (p_widget_focused) {
    p_widget_focused->states.is_absolute && (this->p_abs_activity_widget = p_widget_focused);
    ekg::current.type = p_widget_focused->properties.type;
    ekg::current.unique_id = p_widget_focused->properties.unique_id;

    p_widget_focused->on_event(ekg::io::stage::pre);
    p_widget_focused->on_event(ekg::io::stage::process);
    p_widget_focused->on_event(ekg::io::stage::post);
  }

  if (input.was_pressed) {
    ekg::current.pressed = ekg::current.unique_id;
    (
      // if p_widget_focused != nullptr ? p_widget_focused->properties.type : ekg::type::abstract
      (p_widget_focused && (ekg::current.pressed_type = p_widget_focused->properties.type))
      ||
      (ekg::current.pressed_type = ekg::type::abstract)
    );
  } else if (input.was_released) {
    ekg::current.released = ekg::current.unique_id;
    (
      (p_widget_focused && (ekg::current.released_type = p_widget_focused->properties.type))
      ||
      (ekg::current.released_type = ekg::type::abstract)
    );
  }

  if (
      ekg::current.last != ekg::current.unique_id
      &&
      ekg::current.unique_id != ekg::io::invalid_unique_id
      &&
      (
        input.was_pressed || input.was_released
      )
  ) {
    this->swap_target_collector.unique_id = ekg::current.unique_id;
    ekg::current.last = ekg::current.unique_id;

    ekg::io::dispatch(ekg::io::operation::swap);
    ekg::viewport.redraw = true;
  }
}

void ekg::runtime::update() {
  if (!this->high_frequency_widget_list.empty()) {
    size_t size {};
    for (size_t it {}; it < (size = this->high_frequency_widget_list.size()); it++) {
      if (it >= size) {
        break;
      }

      ekg::ui::abstract *&p_widget {this->high_frequency_widget_list.at(it)};
      p_widget->on_update();

      if (!p_widget->states.is_high_frequency) {
        this->high_frequency_widget_list.erase(
          this->high_frequency_widget_list.begin() + it
        );
      }
    }
  }

  this->service_input.on_update();
  this->service_handler.on_update();

  ekg::log::flush();
}

void ekg::runtime::render() {
  if (ekg::viewport.redraw) {
    ekg::viewport.redraw = false;

    /**
     * The allocator starts here, the GPU data instance
     * and geometry resources are clear/reseted here.
     **/
    this->gpu_allocator.invoke();

    for (ekg::ui::abstract *&p_widgets : this->context_widget_list) {
      if (p_widgets != nullptr && p_widgets->properties.is_alive && p_widgets->properties.is_visible) {
        /**
         * Each time this statement is called, one GPU data is
         * allocated/filled.
         * 
         * The order of rendering depends on which are functions are invoked first.
         * 
         * E.g:
         *  gpu-data-group-1 (on_draw())
         *  gpu-data-group-2
         *  gpu-data-group-3
         * 
         * `gpu-data-group-3` is always hovering all the previous GPU data groups.
         **/
        p_widgets->on_draw();
      }
    }

    /**
     * The allocator does not need to be called all the time,
     * cause it is require more CPU-side calls and GPU-communication/synchronization.
     * 
     * The use of the word "revoke" means that the `invoked signal` ended with a
     * possible `revoke`, i.e all the generated/populated/filled GPU-data is processed and
     * converted to a geometry GPU-buffer. Removing all the unnecessary GPU-data.
     **/
    this->gpu_allocator.revoke();
  }

  /**
   * The allocator renderize an serialized GPU data-list.
   * With some conditions:
   * 
   * 1- A concave shape does not have a rectangle form, and can render text(s) and
   * batched quad(s) at once.
   * 2- A convex shape have a unique form: rectangle.
   * 
   * The convex form is rendered one per one draw call. It is stupid I know, but with this is possible add cool effects.
   * While concave have the possibility of rendering a lot of shapes at once, is not viable to add many custom effects.
   * 
   * The native GPU API calls are apart from allocator, depends on current GPU API selected.
   * Rendering-hardware interface also known as RHI implements different each GPU API.
   * OpenGL 3/4/ES 3, Vulkan, MoltenVK, DirectX11/12.
   * May differ the features or runtime.
   * 
   * But check all allocator comments.
   **/
  this->gpu_allocator.draw();
}

void ekg::runtime::set_current_parent_properties(ekg::properties_t *p_properties) {
  this->p_current_parent_properties = p_properties;
}

ekg::properties_t *ekg::runtime::get_current_parent_properties() {
  return this->p_current_parent_properties;
}

ekg::ui::abstract *ekg::runtime::emplace_back_new_widget_safety(
  ekg::ui::abstract *p_widget
) {
  return this->context_widget_list.emplace_back() =
  (
    this->loaded_widget_list.emplace_back(
      std::unique_ptr<ekg::ui::abstract>(p_widget)
    ).get()
  );
}

ekg::id_t ekg::runtime::generate_unique_id() {
  return ++this->global_id;
}

void ekg::runtime::dispatch_widget_op(
  ekg::ui::abstract *p_widget,
  ekg::io::operation op
) {
  if (p_widget == nullptr) {
    return;
  }

  bool is_cancelled {};
  switch (op) {
  case ekg::io::operation::swap:
    this->swap_target_collector.unique_id = p_widget->properties.unique_id;
    break;
  case ekg::io::operation::reload:
    if (
      !(is_cancelled = p_widget->states.was_reloaded)
    ) {
      this->reload_widget_list.push_back(p_widget);
      p_widget->states.was_reloaded = true;
    }
    break;
  case ekg::io::operation::layout_docknize:
    if (
      !(is_cancelled = p_widget->states.was_layout_docknized)
    ) {
      this->layout_docknize_list.push_back(p_widget);
      p_widget->states.was_layout_docknized = true;
    }
    break;
  case ekg::io::operation::high_frequency:
    if (
      !p_widget->states.is_high_frequency
    ) {
      this->high_frequency_widget_list.push_back(p_widget);
      p_widget->states.is_high_frequency = true;
    }

    is_cancelled = true;
    break;
  }

  if (is_cancelled) {
    return;
  }

  this->service_handler.dispatch_pre_allocated_task(
    static_cast<uint64_t>(op)
  );
}
