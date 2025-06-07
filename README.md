# 🐄 EKG 🐈

EKG is a descriptor-based low-latency memory-safety modular UI-toolkit for desktop-apps, mobile-apps, and high-performance apps.

The purpose of EKG is to be an alternative way to create fancy and low-latency C++ memory-safety GUI-context. EKG is under an experimental-buildable-unstable version, which means no ready-prod release was made before.

The complete article of EKG memory-safety model can be read [here](https://github.com/vokegpu/ekg-docs/blob/master/model/architecture-model.md#architecture-model). Ultimately, EKG does not use raw-ptr(s) or even smart-ptr(s), except for hardware interfaces (platform-base and rendering-api), the entire EKG use of a virtual memory with virtual address which is called `ekg::at_t`, `ekg::at_t` is a virtual-address reference pointer to EKG descriptors, you can read more about [here](https://github.com/vokegpu/ekg-docs/blob/master/model/ui-descriptor-based-model.md#ui-descriptor-based-model).

## GUIs with EKG

### Initializing

This is an example for SDL2 and OpenGL 4.5, for a complete desired program [looks here](https://github.com/vokegpu/ekg-sandbox).

```cpp
ekg::ft_library ft_library {};
FT_Init_FreeType(&ft_library);

ekg::runtime_properties_info_t runtime_properties_info {
  .default_font_path_text = "./comic-mono.ttf",
  .default_font_path_emoji = "./twemoji.ttf",
  .p_platform_base = new ekg::sdl2(app.p_sdl_win),
  .p_gpu_api = new ekg::opengl(),
  .ft_library = ft_library
};

ekg::runtime_t ekg_runtime {};
ekg::init(
  runtime_properties_info,
  &ekg_runtime
);

while (true) {
  while (SDL_PollEvent(&sdl_event)) {
    ekg::sdl2_poll_event(sdl_event);
  }

  ekg::update();

  // clear screen and previous buffers

  ekg::gui.ui.dt = 0.016f;
  ekg::render();

  // swap buffers
}
```

### Widgets

A simple frame using the basic of `ekg::make<t>`!
```cpp
ekg::make<ekg::stack_t>(
  ekg::stack_t meow {
    .tag = "moo"
  }
);

ekg::frame_t frame_template {
  .tag = "meows",
  .rect = {20.0f, 20.0f, 200.0f, 200.0f},
  .drag = ekg::dock::top,
  .resize = ekg::dock::left | ekg::dock::right | ekg::dock::bottom
};

auto &my_frame = ekg::make<ekg::frame_t>(frame_template);
```

## Building

Dependencies for building binary:
- [FreeType](https://freetype.org/)
- [GLEW](https://glew.sourceforge.net/)
- [SDL2](https://www.libsdl.org/)
- [Ninja](https://ninja-build.org/)
- [CMake](https://cmake.org/)

Note: EKG is not SDL2 or GLEW fixed, this is for compile the entire library, for using the library you will not need to compile SDL2 or even GLEW with EKG, you can choose soon Vulkan and GLFW.

Run the following commands:
```cpp
cmake -S . -B ./cmake-build -G Ninja -D CMAKE_BUILD_TYPE=Release
cmake --build ./cmake-build
```

Supported compilers:
- GNU-MinGW64 stable buildable
- GNU-G++ stable buildable
- Clang++ stable buildable

Supported video APIs:
- OpenGL 3.1~4.6
- OpenGL ES 3
- Emscripten WebGL 2 (OpenGL ES 2) (require building EKG with flag `-D EKG_EMSCRIPTEN_BUILD_TYPE=1`)

Supported platforms:
- SDL2

## Contributing

Contributing for EKG is nicely welcome, please confer [EKG docs and read the model architecture](https://github.com/vokegpu/ekg-docs?tab=readme-ov-file#ekg-technical-details) for prevent ~useless~ pull requests.
