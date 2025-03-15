#include "ekg/ui/frame/frame_widget.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/context.hpp"
#include "ekg/draw/shape.hpp"

void ekg::ui::frame::on_create() {
  ekg::ui::abstract::on_create();
}

void ekg::ui::frame::on_destroy() {

}

void ekg::ui::frame::on_reload() {

}

void ekg::ui::frame::on_event(ekg::io::stage stage) {
  switch (stage) {
    case ekg::io::stage::pre: {
      ekg::ui::abstract::on_event(ekg::io::stage::pre);
      break;
    }

    case ekg::io::stage::process: {
      ekg::rect_t<float> &rect {this->get_abs_rect()};
      ekg::input_t &input {ekg::p_core->service_input.input};
      ekg::flags_t resize_over_dock {};

      this->action(
        input.has_motion && this->states.is_hover && (ekg::timing_t::second > ekg::tweaks.task_latency),
        ekg::action::motion
      );

      if (
          input.was_pressed
          &&
          (
            (this->descriptor.drag_dock != ekg::dock::none && ekg::fire("frame-drag"))
            ||
            (this->descriptor.resize_dock != ekg::dock::none && ekg::fire("frame-resize"))
          )
        ) {

        ekg::vec2_t<float> margin {
          this->descriptor.theme.margin_actions_offset,
          this->descriptor.theme.margin_actions_offset
        };

        ekg::scale_docker_by_margin<float>(
          this->docker_drag,
          margin,
          rect
        );

        const float margin_factor {4.0f};
        ekg::scale_docker_by_margin<float>(
          this->docker_resize,
          margin / margin_factor,
          rect
        );

        this->target_dock_drag = ekg::vec2_collide_docker_if<float>(
          static_cast<ekg::vec2_t<float>>(input.interact),
          this->docker_drag,
          this->descriptor.drag_dock
        );
      
        this->target_dock_resize = ekg::vec2_collide_docker_if<float>(
          static_cast<ekg::vec2_t<float>>(input.interact),
          this->docker_resize,
          this->descriptor.resize_dock
        );

        resize_over_dock = this->target_dock_resize;

        this->rect_delta.x = input.interact.x - rect.x;
        this->rect_delta.y = input.interact.y - rect.y;
        this->rect_delta.w = rect.w;
        this->rect_delta.h = rect.h;
        this->rect_cache = rect;

        this->states.is_active = this->target_dock_drag != ekg::dock::none || this->target_dock_resize != ekg::dock::none;
        this->states.is_absolute = this->states.is_active;

        this->action(
          true,
          ekg::action::press
        );
      } else if (input.has_motion && this->states.is_active) {
        ekg::rect_t<float> new_rect {rect};
        ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};        

        resize_over_dock = this->target_dock_resize;

        if (this->target_dock_drag != ekg::dock::none && this->target_dock_resize == ekg::dock::none) {
          this->action(
            ekg::timing_t::second > ekg::tweaks.task_latency,
            ekg::action::drag
          );

          new_rect.x = interact.x - this->rect_delta.x;
          new_rect.y = interact.y - this->rect_delta.y;
          ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::size_all;
        }

        if (this->target_dock_resize != ekg::dock::none) {
          this->action(
            ekg::timing_t::second > ekg::tweaks.task_latency,
            ekg::action::resize
          );

          if (ekg::has(this->target_dock_resize, ekg::dock::left)) {
            interact.x = ekg::min_clamp<float>(interact.x, this->rect_delta.x);
            new_rect.x = ekg::max_clamp<float>(interact.x - this->rect_delta.x, this->rect_cache.x + this->rect_cache.w - ekg::viewport.minimum_possible_size);
            new_rect.w = (this->rect_cache.x + this->rect_cache.w) - new_rect.x;
          }

          if (ekg::has(this->target_dock_resize, ekg::dock::right)) {
            new_rect.w = ekg::min_clamp<float>(this->rect_delta.w + ((interact.x - new_rect.x) - this->rect_delta.x), ekg::viewport.minimum_possible_size);
          }

          if (ekg::has(this->target_dock_resize, ekg::dock::top)) {
            interact.y = ekg::min_clamp<float>(interact.y, this->rect_delta.y);
            new_rect.y = ekg::max_clamp<float>(interact.y - this->rect_delta.y, this->rect_cache.y + this->rect_cache.h - ekg::viewport.minimum_possible_size);
            new_rect.h = (this->rect_cache.y + this->rect_cache.h) - new_rect.y;
          }

          if (ekg::has(this->target_dock_resize, ekg::dock::bottom)) {
            new_rect.h = ekg::min_clamp<float>(this->rect_delta.h + ((interact.y - new_rect.y) - this->rect_delta.y), ekg::viewport.minimum_possible_size);
          }
        }

        ekg::clamp_rect_by_square<float>(
          new_rect,
          ekg::viewport.minimum_possible_size
        );

        if (rect != new_rect) {
          if (this->properties.p_parent != nullptr) {
            this->rect.x = new_rect.x - this->p_parent_rect->x;
            this->rect.y = new_rect.y - this->p_parent_rect->y;
          } else {
            this->p_parent_rect->x = new_rect.x;
            this->p_parent_rect->y = new_rect.y;
          }

          this->rect.w = new_rect.w;
          this->rect.h = new_rect.h;

          if (this->target_dock_resize != ekg::dock::none) {
            this->states.is_targeting_absolute_parent = true;
            ekg::p_core->dispatch_widget_op(
              this,
              ekg::io::operation::layout_docknize
            );
          }

          ekg::viewport.redraw = true;
        }
      } else if (
        this->states.is_hover
        &&
        this->descriptor.resize_dock != ekg::dock::none
        &&
        !this->states.is_active
      ) {
        ekg::vec2_t<float> margin {
          this->descriptor.theme.margin_actions_offset,
          this->descriptor.theme.margin_actions_offset
        };

        const float margin_factor {4.0f};
        ekg::scale_docker_by_margin<float>(
          this->docker_resize,
          margin / margin_factor,
          rect
        );

        resize_over_dock = ekg::vec2_collide_docker_if<float>(
          static_cast<ekg::vec2_t<float>>(input.interact),
          this->docker_resize,
          this->descriptor.resize_dock
        );
      }

      if (resize_over_dock != ekg::dock::none) {
        bool is_top {ekg::has(resize_over_dock, ekg::dock::top)};
        bool is_bottom {ekg::has(resize_over_dock, ekg::dock::bottom)};
        bool is_left {ekg::has(resize_over_dock, ekg::dock::left)};
        bool is_right {ekg::has(resize_over_dock, ekg::dock::right)};

        if ((is_top && is_left) || (is_bottom && is_right)) {
          ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::size_nwse;
        } else if ((is_top && is_right) || (is_bottom && is_left)) {
          ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::size_nesw;
        } else if (is_top || is_bottom) {
          ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::size_ns;
        } else if (is_left || is_right) {
          ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::size_we;
        }
      }

      if (input.was_released) {
        if (this->states.is_active) {
          this->states.is_absolute = false;

          this->action(
            this->states.is_hover,
            ekg::action::release
          );
        }

        this->target_dock_resize = ekg::dock::none;
        this->target_dock_drag = ekg::dock::none;
        this->states.is_active = false;
      }

      break;
    }

    case ekg::io::stage::post: {
      ekg::ui::abstract::on_event(ekg::io::stage::post);
      break;
    }
  }
}

void ekg::ui::frame::on_update() {

}

void ekg::ui::frame::on_draw() {
  ekg::rect_t<float> &rect {
    this->get_abs_rect()
  };

  ekg::draw::sync_scissor(this->scissor, rect, this->p_parent_scissor_rect);
  EKG_ASSERT_SCISSOR();

  ekg::draw::rect(
    rect,
    this->descriptor.theme.background,
    ekg::draw_mode::filled,
    this->properties.layer.at(static_cast<size_t>(ekg::layer::background))
  );

  ekg::draw::rect(
    this->docker_drag.top,
    this->descriptor.theme.border,
    ekg::draw_mode::filled
  );

  ekg::draw::rect(
    rect,
    this->descriptor.theme.outline,
    ekg::draw_mode::outline
  );
}
