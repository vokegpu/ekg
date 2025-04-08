# 🐄 EKG 🐈

EKG is a descriptor-based low-latency UI-toolkit for desktop-apps, mobile-apps, and high-performance apps. Offers multi-support for platforms and graphics API(s).

**Note: EKG is not stable yet.**

Contributing for EKG is nicely welcome, just take a look on [code of conduct](./CODE-OF-CONDUCT) and [VokeGpu library coding-guide style](https://github.com/vokegpu/code-of-conduct-and-style-guide).

### GUIs with EKG

Creating a simple frame:

```C++
ekg::make(
  ekg::frame_t {
    .tag = "first frame",
    .rect = {20.0f, 20.0f, 200.0f, 200.0f},
    .drag_dock = ekg::dock::top,
    .resize_dock = ekg::dock::left | ekg::dock::bottom | ekg::dock::right
  }
);
```

![image](https://github.com/user-attachments/assets/1629c5ae-fa50-47f4-8900-65d11479df64)

Adding buttons:

```C++
ekg::button_t button {};

button.tag = "bt-1";
button.text = "moo moo";
button.text_dock = ekg::dock::left;
button.dock = ekg::dock::left;
ekg::make(button);

button.tag = "bt-2";
button.text = "owlf olwf";
button.text_dock = ekg::dock::center;
button.dock = ekg::dock::next | ekg::dock::fill;
ekg::make(button);

button.tag = "bt-3";
button.text = "oi amo gatinhos";
ekg::make(button);
```

![image](https://github.com/user-attachments/assets/5f510ef6-e51b-4ca0-a5fc-1d36582bb055)

You can dynamic change the value, text or any display output via safety-reference:

```C++
button.tag = "bt-3";
button.text = "oi amo gatinhos";
ekg::button_t &bla = ekg::make(button);
```

And then dynamic trigger/set value from fields.

```C++
bla.value = true; // set button trigger value forced
bla.text = "clicked here times: " + std::to_string(button_click_times); // set the text dynamically
```

Others widgets are being re-implemented in this new memory-model, also new future widgets.

May you want to know about this new memory-model or about EKG more, go here.  
[EKG Docs](https://github.com/vokegpu/ekg-docs)

### Initializing EKG

EKG is a modular library, which does not necessary have one unique base, for example the platform used, supporting SDL2, GLFW and soon others.

```cpp
ekg::runtime_property_t ekg_runtime_property {
  .font_path = "default.ttf",
  .font_path_emoji = "default-emoji.ttf",
  .p_gpu_api = new ekg::opengl(),
  .p_os_platform = new ekg::sdl(p_win_sdl)
};

ekg::runtime ekg_runtime {};
ekg::init(&ekg_runtime, &ekg_runtime_property);
```

Supported GPU API(s): OpenGL3+, OpenGLES3.

Supported platform(s): SDL2, GLFW.
