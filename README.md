# Project_AK

**Alpha 0.04e**

A 2D game engine built from scratch in **C++20**, with **OpenGL 4.6 Core (DSA)** rendering and window/input handling via **SDL3**.

The goal of this project isn't a one-off game, but to progressively build a **proper, reusable 2D engine that's fully understood end to end**: render pipeline, object/component system, scenes, asset management, and everything needed to support multiple games on top of the same foundation.

The engine's systems are largely in place; the primary goal now is reaching a **v1 release**, gated not by missing features but by validating everything working together in a real, complete game. The public API is fairly stable, with infrequent breaking changes.

---

## Current state

The engine already has a working foundation that's still being iterated on:

- **Batched renderer** (`Batcher`): a single draw call groups quads, circles, triangles, lines, polygons, and textured sprites. Built on **OpenGL 4.6 DSA**, with a persistent mapped, triple-buffered VBO (per-section `GLsync` fences to avoid GPU stalls), a `FrameDataGPU` UBO (`std140`) for view/projection, and a per-vertex `EffectMode` attribute. Sampler binding uses shader-side `layout(binding=N)` instead of string-based uniform lookups. Up to 16 simultaneous texture slots, plus a reserved `TextureArray` sampler unit for atlas-backed sprites. A per-object `IMaterial` can override the default shader for its own draws.
- **Object + component system**: `Object` as the base entity with a lifecycle (`Update`/`Draw`) and a type-safe `ComponentStorage` (based on `std::type_index`). Components include `ITransform`, `IStats`, `ISprite`, `IAnimator`, `IBoundingBox`, `IColor`, and `IMaterial`.
- **`ObjectPool`**: an object pool with a dense array + sparse map and `ObjectID` handles, designed for O(1) add/remove (swap-and-pop) without invalidating external references.
- **Layered render queue** (`RenderEntry`): support for sorting draws by `layer` (then by shader), avoiding unnecessary full re-sorts every frame.
- **2D collision system**: Using `AABB` System, can resolver using `IBoundingBox` or using `Rectangle`, `Point` and `Circle` collision.
- **Atlas + TextureArray asset pipeline**: `AssetsManager` loads sprite sheets into a GPU `TextureArray` and parses their layout from a JSON atlas file (`AtlasData`/`ParseAtlasJSON`) into either named `animations` (frame strips, for `IAnimator`) or named `tiles` (single-cell lookups, for tilemaps and static atlas sprites).
- **Animation**: `IAnimator` advances an atlas-backed `ISprite`'s frame over time (speed/step-controlled), driven by named animation regions from the atlas JSON.
- **Tile maps**: `TileMap` (`Engine/Map/TileMap`) loads a tile grid + a tile-atlas JSON, resolving each cell to a UV once at load time; `Scene::AddTileMap`/`RenderTileMaps` let a scene keep tile maps sorted by layer and interleave their draws with the object render queue (e.g. background tiles before the player, foreground tiles after). Tiled layer names map to engine draw layers automatically (`TiledLayerToEngineLayer` in `Game.hpp`), so authoring a map in Tiled doesn't need per-map C++ wiring.
- **Text rendering**: `stb_truetype` is wired up via `BakerFont` (glyph baking) and a `TextAPI` for drawing text through the batcher.
- **GUI**: basic `Button` and `Label` widgets built on top of the rendering/input systems.
- **Input**: keyboard, mouse (including wheel), and gamepad, all polled via `PollEvent`.
- **Data**: `CSVManager` (rapidcsv/csv.h) for loading tabular data such as dialogue.
- **Scene system**: a `Scene` interface (`Init`/`Update`/`Inputs`/`Render`) managed by a `ScenesManager`, each owning its own `Camera2D` and tile maps.
- **Audio**: `MusicManager` and `SFXManager` (SDL3_mixer-backed) handle music tracks and pooled, overlapping sound effects respectively, each with per-key volume control.
- **Timing**: `Services::Clock` (steady_clock-based), with a clamped delta time for gameplay/physics and an unclamped accumulator for time-driven shader effects.
- **Service Locator** (`Services`): controlled global access to `AssetsManager`, `ScenesManager`, `ShaderManager`, `MusicManager`, and `SFXManager`.
- **Application layer** (`GameLayer`): decouples the engine from the actual game; the current executable (`Game`).
- **Asset loading**: `stb_image` and `stb_image_write` for textures; `stb_truetype` for baked fonts.
- **Early serialization**: `Object::Save`/`Load` via `ObjectState`, the base for a world-saving system (`WorldSaver`).
- **CI/CD**: multi-platform GitHub Actions coverage (Ubuntu with GCC + Clang, Windows with MSVC).
## Where it's headed

The primary goal right now is completing a small but complete **demo/integration game** — a menu plus two levels with collisions — designed to exercise as many engine systems together as possible. This demo is the main gate before considering a v1 release; the gap isn't missing systems, it's that no full game has been built with them yet, only isolated demos and tests.

Open integration risks being worked through as part of the demo:

- Scene transition cleanup (`ObjectPool` reset behavior, audio/asset cache state across scenes).
- Initialization order from menu to level, ensuring systems come up in the right sequence.
- `CSVManager` + `TileMap` + Collision running together in a real game context for the first time.
Beyond the demo/v1 push:

- Expanding `TileMap` (multi-atlas maps, more editor-friendly authoring).
- Remappable input (beyond the current `PollEvent` polling).
- Editor or support tooling (object inspection, asset hot-reload).
- Improving the world save/load system started in `WorldSaver`.
- 3D support and multi-camera work (at which point a deferred `ForceFlush()` becomes relevant).
## Tech stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Rendering | OpenGL 4.6 Core (DSA) |
| Shading | GLSL 4.60 |
| Window / Input / Platform | SDL3 (fetched via `FetchContent`) |
| Audio | SDL3_mixer (fetched via `FetchContent`) |
| GL function loading | GLAD (vendored in `external/glad`) |
| Images / fonts | stb_image, stb_image_write, stb_truetype (via `BakerFont`) |
| Data | rapidcsv/csv.h |
| Math | GLM |
| JSON | nlohmann_json (fetched via `FetchContent`) |
| Build system | CMake 3.25+ (Ninja Multi-Config) |

## Repository structure

```
Project_AK/              # Root folder of the project.
├── assets/              # Game resources (sprite atlases, tile maps, etc.).
│   └── Atlas/           # Atlas textures + JSON layouts (animations and/or tiles).
├── external/glad/       # Vendored OpenGL loader.
├── shaders/             # GLSL shaders (texture.vs / texture.fs), And all you shaders.
├── src/
│   ├── Main/            # Entry point (main.cpp).
│   ├── Engine/          # The engine itself,
│   │   ├── Component/   # Components and storage (ECS-like).
│   │   ├── Extern/      # Vendored third-party headers (stb, rapidcsv).
│   │   ├── GUI/         # Button, Label, TextBox, Scissors, Scroll.
│   │   ├── Inputs/      # Event/input handling, Mouse.
│   │   ├── Layer/       # GameLayer and Scene (base interfaces).
│   │   ├── Map/         # TileMap: tile-grid loading and rendering.
│   │   ├── Modules/     # Bakers and others stuff for the engine. 
│   │   ├── Object/      # Object and ObjectPool.
│   │   ├── Physics/     # Collision: IBoundingBox broad-phase + resolution.
│   │   ├── Render/      # Batcher, Shader, Image, AtlasData, TextureArray, Color, Vertex...
│   │   ├── Services/    # AssetsManager, ScenesManager, ShaderManager, MusicManager, SFXManager, WorldSaver, Clock, CSVManager.
│   │   ├── Text/        # BakerFont, TextAPI (stb_truetype-backed text rendering).
│   │   └── Utils/       # Vector2, Rects, Config, Log, Path.
│   └── Game/            # Game layer on top of the engine.
│       ├── Scenes/      # MainMenu, Demo.
│       ├── GameWrapper/ # Communication to the engine, to render. (Game.cpp & Game.hpp).
│       └── Systems/     # Logic for you game.
├── CMakeLists.txt       # Root CMakeLists, config everything and download all libraries.
├── runner.sh            # Quick build + run.
└── commands.cmd         # Reference CMake commands.
```

## Building

### Requirements

Common to all platforms:

- CMake 3.25 or higher
- Ninja
- A C++20-capable compiler
- Drivers with OpenGL 4.6 support
SDL3, SDL3_mixer, GLM, and nlohmann_json are downloaded and built automatically via `FetchContent`, so there's no need to install them separately.

Per platform:

- **Linux**: GCC or Clang, plus X11 development headers (needed for SDL3's X11 video driver). On Debian/Ubuntu:
```bash
 sudo apt-get install build-essential cmake ninja-build \
   libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
   libwayland-dev libxkbcommon-dev libdrm-dev libgbm-dev \
   libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev \
   libasound2-dev libpulse-dev libdbus-1-dev libibus-1.0-dev libudev-dev
```
- **Windows**: MSVC (Visual Studio Build Tools or Visual Studio with the "Desktop development with C++" workload), plus CMake and Ninja (both installable via the Visual Studio installer, `winget`, or standalone). The root `CMakeLists.txt` statically links the MSVC runtime, so the built `.exe` doesn't need the VC++ Redistributable installed on the target machine.
- **macOS**: Xcode Command Line Tools (`xcode-select --install`) for Clang and the system SDK, plus CMake and Ninja (e.g. `brew install cmake ninja`). Note: macOS's OpenGL driver stack tops out at 4.1 Core — running Project_AK's OpenGL 4.6 DSA path on macOS is not currently supported. (Thinking to add Metal)
### Steps

```bash
mkdir build && cd build
cmake -G "Ninja Multi-Config" ..
cd ..
cmake --build build --config Debug --target Project_AK
./build/Debug/Project_AK   # Windows: build/Debug/Project_AK.exe
```

Or, more directly, using the included script (works as-is on Linux, macOS, and Windows via a bash shell like Git Bash — it detects the `.exe` suffix automatically):

```bash
./runner.sh
```

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.

