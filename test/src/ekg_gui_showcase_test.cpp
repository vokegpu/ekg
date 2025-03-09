#include <iostream>

#include "application.hpp"
#include <ekg/os/ekg_opengl.hpp>
#include <ekg/draw/shape.hpp>

application_t app {};

int32_t main(int32_t, char**) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  app.p_sdl_win = {
    SDL_CreateWindow(
      "Pompom",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      1280,
      720,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    )
  };

  SDL_Event sdl_event {};
  SDL_GLContext sdl_gl_context {SDL_GL_CreateContext(app.p_sdl_win)};
  glewInit();

  app.is_running = true;
  ekg::vec3_t<float> clear_color {};

  ekg::runtime_property_t ekg_runtime_property {
    .font_path = "./comic-mono.ttf",
    .font_path_emoji = "./twemoji.ttf",
    .p_gpu_api = new ekg::opengl(),
    .p_os_platform = new ekg::sdl(app.p_sdl_win)
  };

  ekg::runtime ekg_runtime {};
  ekg::init(&ekg_runtime, &ekg_runtime_property);

  ekg::viewport.redraw = true;
  ekg::flags_t resize {
    ekg::dock::left | ekg::dock::bottom | ekg::dock::right
  };

  ekg::stack_t bla {
    .tag = "idk",
    .children = {
      ekg::make<ekg::frame_t>(
        {.tag = "bla", .rect = {20.0f, 20.0f, 200.0f, 200.0f}, .resize_dock = resize, .drag = ekg::dock::top}
      )
    }
  };
  

  while (app.is_running) {
    while (SDL_PollEvent(&sdl_event)) {
      if (sdl_event.type == SDL_QUIT) {
        app.is_running = false;
      }

      ekg::sdl_poll_event(sdl_event);
    }

    ekg::update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
    glViewport(0.0f, 0.0f, ekg::viewport.w, ekg::viewport.h);

    ekg::viewport.dt = 0.016f;
    ekg::render();

    SDL_GL_SwapWindow(app.p_sdl_win);

    if (app.vsync) {
      SDL_Delay(16);
    }
  }

  return 0;
}
