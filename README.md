# Project_AK

**Alpha 0.05**

A 2D game engine built from scratch in **C++20**, with **OpenGL 4.6 Core (DSA)** rendering and window, input and audio through **SDL3**.

The goal isn't a one-off game, but a **reusable 2D engine that is understood end to end**: render pipeline, object/component system, scenes, maps, UI, assets and everything needed to build more than one game on the same foundation. `src/Game` is the sample game that lives on top of the engine and is used to try every feature out.

The main systems are in place. The road to **v1** is not about missing features but about validating them together in a small, complete game.

---

## Current state

### Rendering
- **Batched renderer** (`Batcher`): quads, circles, triangles, lines, polygons and textured sprites go into one dynamic VAO/VBO/EBO and are drawn with one call per flush. Vertices are built in CPU arrays and uploaded with `glNamedBufferSubData`. View/projection, time and screen size live in a `std140` UBO; samplers are bound with `layout(binding=N)` in the shaders. Up to 16 texture slots plus a reserved `TextureArray` unit for atlas sprites. A per-object `IMaterial` can override the default shader, and a per-vertex effect mode drives shader effects.
- **Draw order** (`RenderEntry`): objects are sorted by layer, then render space, then shader. The queue is cached and only rebuilt when objects are added or removed. Layer convention in `Render/Layers.hpp` (ground, collision, world, player, top, fx, UI).
- **World and screen space** (`RenderSpace`): each object is drawn either with the camera (`World`) or in fixed screen pixels (`Screen`: HUD, menus), in the same scene.
- **Camera** (`Camera2D`): position, zoom, smooth `Follow(position, dt)`, `ScreenToWorld`. A scene has a main camera and can switch to others.
- **Text**: fonts baked with `stb_truetype` (`BakerFont`) and drawn through the batcher by `TextAPI`. Text is **UTF-8** (accents, ñ, ¿¡), decoded to codepoints (`Text/Utf8.hpp`).

### Objects and components
- **`Object`**: base entity with `Update`/`Draw`, a transform, a layer, a render space and a visibility flag (hidden objects keep updating but are not drawn).
- **Components** in a type-safe `ComponentStorage` (`std::type_index`): `ITransform`, `ISprite`, `IAnimator`, `IBoundingBox`, `ICircleBox`, `IColor`, `IMaterial`, `IText`. Games add their own by deriving from `IComponent`.
- **`ObjectPool`**: dense array + sparse map with `ObjectID` handles, O(1) add/remove (swap-and-pop). Game code keeps `ObjectID`s, not raw pointers.
- **Pool systems** (`Object/PoolSystems.hpp`): free functions that run a pool: `UpdateObjects`, `DispatchPointer` (mouse, top layer first) and `RenderObjects` (all, or a layer range).
- **Serialization**: `Object::Save`/`Load` round-trip through `ObjectState`, covering every native component. Game components are registered with `ComponentSerializers::Register<T>("key")` (the engine never names them); runtime-only ones are marked with `Ignore<T>()`. `WorldSaver` writes the result as CBOR.

### Scenes
- **`Scene`** is only the interface (`Init`/`Destroy`/`Inputs`/`Update`/`UpdateFixed`/`Render`) plus its pool, main camera and ID. Everything else is composed by the scene that needs it.
- **`ScenesManager`**: scenes keyed by ID, with deferred changes. An unknown ID is reported and ignored; the current scene is only destroyed when the change succeeds. Scenes can also be removed for good (`DestroyScene`).

### Maps
- **Importer pipeline**: the engine never knows Tiled. `IMP::TiledLoader` (`src/Importers/`) turns Tiled JSON into a format-neutral `ENG::MapData`.
- **`SceneMap`**: builds the drawable `TileMap`s (orthogonal and isometric, UVs resolved once) and a `CollisionGrid` from `MapData`. What each layer means is data, not code: a `MapLayerRules` table keyed by the layer's Tiled class or name, with per-layer overrides.
- Tile layers and objects are interleaved by layer range (background tiles, objects, foreground tiles, UI).

### Input and UI
- **`PollEvent`**: keyboard, mouse (buttons, wheel) and Press/Down/Release queries per frame.
  - **Click consumption**: an object that takes a click claims it until release, so objects below and the world don't see it.
  - **Text input**: SDL-composed UTF-8 text (keyboard layout, accents), editing keys with OS repeat and modifiers; the keyboard is captured while typing.
- **Widgets** (`GUI/`), living in the scene pool like any object:
  - `Button`: detects and consumes its own click, runs a callback.
  - `TextBox`: focus, placeholder, caret with ←/→/Home/End, Backspace/Delete by character, Ctrl+A select all, horizontal scroll, max length, submit callback on Enter.
  - `Label`: text with alignment, background and a letter-by-letter reveal (`TextReveal`).

### Physics
- Free collision functions (`Physics/Collision.hpp`): AABB, circle, point/rectangle, and object vs. `CollisionGrid` for tile maps.

### Services and utilities
- **Service locator** (`Services`): `AssetsManager` (images into a GPU `TextureArray` + atlas JSON with animations and tiles), `FontManager`, `ShaderManager`, `ScenesManager`, `MusicManager` and `SFXManager` (SDL3_mixer, pooled overlapping effects), `ClockManager` (delta time clamped to 0.05 s, total time, FPS), `CSVManager` (rapidcsv; used for dialog texts, one column per language), `WorldSaver`.
- **Application layer** (`GameLayer`): the only seam between the engine and a game.
- **Utilities**: `Vector2`, `Log` (`LOG_INFO`/`LOG_DEBUG`/`LOG_ERROR`/`LOG_FATAL`), `Path` (everything relative to the executable), interpolation, Bézier.

### Sample game (`src/Game`)
- **MainMenu**: title, name `TextBox`, "Jugar" and "Salir" buttons.
- **GameScene**: an isometric Tiled map with collisions, a player that moves on the grid, a following camera, a dialog box (E) fed from `assets/Dialogs/dialog.csv`, and a collision debug view (G).

---

## Where it's headed

The next milestone is a small but **complete game** (menu plus a couple of levels) that exercises the systems together; it is the gate before v1. Open work, tracked in [`docs/Diagnostico.md`](./docs/Diagnostico.md) (Spanish):

- A real fixed time step for `UpdateFixed`, pause, and UI anchors when the window is resized.
- Physics clean-up: a single rule for moving objects (always through `SetPosition`) and the remaining collision fixes.
- Gamepad support (the `PollEvent` gamepad queries are still stubs) and remappable input.
- Clipboard and mouse selection in `TextBox`.
- Tests for the GL-free parts (`ObjectPool`, collisions, `CollisionGrid`, UTF-8, serialization, importer) and a Debug job in CI with sanitizers.
- Later: more map tooling, editor/inspection tools, asset hot-reload.

---

## Tech stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Rendering | OpenGL 4.6 Core (DSA), GLSL 4.60 |
| Window / Input / Platform | SDL3 (fetched via `FetchContent`) |
| Audio | SDL3_mixer (fetched via `FetchContent`) |
| GL loading | GLAD (vendored in `external/glad`) |
| Images / fonts | stb_image, stb_image_write, stb_truetype |
| Data | rapidcsv |
| Math | GLM (fetched via `FetchContent`) |
| JSON / CBOR | nlohmann_json (fetched via `FetchContent`) |
| Maps | Tiled (JSON export), read by `IMP::TiledLoader` |
| Build system | CMake 3.25+ (Ninja Multi-Config) |

---

## Repository structure

```
Project_AK/
├── assets/              # Game resources: Atlas/, Font/, Map/ (Tiled), Dialogs/ (CSV), Music/, SFX/...
├── config/              # eng.json: window title, size, fullscreen.
├── docs/                # Diagnostico.md: project review, open bugs and roadmap (Spanish).
├── external/glad/       # Vendored OpenGL loader.
├── shaders/             # GLSL shaders.
├── src/
│   ├── Main/            # Entry point (main.cpp): reads config, runs the Engine with the Game.
│   ├── Engine/          # The engine.
│   │   ├── Component/   # Components, ComponentStorage, ComponentSerializers.
│   │   ├── Extern/      # Vendored headers (stb, rapidcsv).
│   │   ├── GUI/         # Button, Label, TextBox, TextReveal.
│   │   ├── Inputs/      # PollEvent, Mouse.
│   │   ├── Layer/       # GameLayer and Scene (base interfaces).
│   │   ├── Map/         # MapData, SceneMap, TileMap, CollisionGrid, MapLayerRules.
│   │   ├── Modules/     # BakerFont (font baking).
│   │   ├── Object/      # Object, ObjectPool, PoolSystems.
│   │   ├── Physics/     # Collision functions.
│   │   ├── Render/      # Render, Batcher, Camera2D, shaders, images, atlases, layers.
│   │   ├── Services/    # Service locator and managers.
│   │   ├── Text/        # Font, TextAPI, UTF-8 helpers.
│   │   └── Utils/       # Vector2, Log, Path, Config, interpolation.
│   ├── Importers/       # Format importers outside the engine (Tiled).
│   └── Game/            # Sample game: Game (GameLayer), Scenes, Components, Map, Dialog, Systems.
├── CMakeLists.txt       # Root CMake: options, dependencies (FetchContent), post-build copy.
├── CHANGELOG.md         # Version history.
├── CLAUDE.md            # Architecture notes for contributors and AI tools.
├── runner.sh            # Configure (if needed), build Debug and run, logging to logs/.
└── valgrindTest.sh      # Separate build without sanitizers, run under Valgrind.
```

---

## Building

### Requirements

- CMake 3.25 or higher
- Ninja
- A C++20 compiler
- A GPU driver with OpenGL 4.6

SDL3, SDL3_mixer, GLM and nlohmann_json are downloaded and built automatically via `FetchContent`.

Per platform:

- **Linux**: GCC or Clang, plus the X11/Wayland development headers SDL3 needs. On Debian/Ubuntu:
  ```bash
  sudo apt-get install build-essential cmake ninja-build \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
    libwayland-dev libxkbcommon-dev libdrm-dev libgbm-dev \
    libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev \
    libasound2-dev libpulse-dev libdbus-1-dev libibus-1.0-dev libudev-dev
  ```
- **Windows**: MSVC (Visual Studio or Build Tools with "Desktop development with C++"), CMake and Ninja. The MSVC runtime is linked statically, so the `.exe` doesn't need the VC++ Redistributable.
- **macOS**: not supported for now: macOS's OpenGL stops at 4.1 Core and the engine uses 4.6 DSA.

### Steps

The build is multi-config: always pass `--config Debug|Release|RelWithDebInfo`.

```bash
mkdir build && cd build
cmake -G "Ninja Multi-Config" ..
cd ..
cmake --build build --config Debug --target Project_AK
./build/Debug/Project_AK          # Windows: build\Debug\Project_AK.exe
```

Or use the script (Linux, macOS, or Git Bash on Windows), which configures if needed, builds Debug, runs the game and saves the output in `logs/`:

```bash
./runner.sh
```

Notes:
- **Debug builds use ASan/UBSan** (`ENABLE_SANITIZERS=ON`). They are slower: judge smoothness and performance in Release or RelWithDebInfo.
- **Valgrind**: `./valgrindTest.sh` uses a separate `build-valgrind/` tree without sanitizers.
- The build copies `assets/`, `shaders/` and `config/` next to the executable; paths are resolved relative to it.
- Sources are collected with `GLOB_RECURSE`: after adding or removing a `.cpp`, re-run the CMake configure step.
- CI (GitHub Actions) builds Release on Ubuntu (GCC, Clang) and Windows (MSVC).

---

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details. Third-party notices are in [THIRD_PARTY_NOTICES.md](./THIRD_PARTY_NOTICES.md).
