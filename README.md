# 🐄 EKG 🐈

EKG is a descriptor-based low-latency modular UI-toolkit for desktop-apps, mobile-apps, and high-performance apps. 

Platforms: SDL2, GLFW.  
GPU APIs: OpenGL3+, OpenGLES3.

EKG is under a experimental-buildable-unstable version, which means no ready-prod release was made before.

## GUIs with EKG

### Initializing

This is an example for SDL2 and OpenGL 4.5, for a complete desired program, [looks here](https://github.com/vokegpu/ekg-sandbox).

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
```

### Widgets

A simple frame using the basic of `ekg::make<t>`!
```cpp
ekg::stack_t meow {
  .tag = "moo"
};

ekg::frame_t frame_template {
  .tag = "meows",
  .rect = {20.0f, 20.0f, 200.0f, 200.0f},
  .drag = ekg::dock::top,
  .resize = ekg::dock::left | ekg::dock::right | ekg::dock::bottom
};

auto &my_frame = ekg::make<ekg::frame_t>(frame_template);
```` 

## Contributing

Contributing for EKG is nicely welcome, please confer [EKG docs and read the model architecture](https://github.com/vokegpu/ekg-docs?tab=readme-ov-file#ekg-technical-details) for prevent ~useless~ pull requests.
