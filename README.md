# Project_AK

A 2D game engine built from scratch in **C++20**, with **OpenGL 3.3 Core** rendering and window/input handling via **SDL3**.

The goal of this project isn't a one-off game, but to progressively build a **proper, reusable 2D engine that's fully understood end to end**: render pipeline, object/component system, scenes, asset management, and everything needed to support multiple games on top of the same foundation.

---

## Current state

The engine already has a working foundation that's still being iterated on:

- **Batched renderer** (`Batcher`): a single draw call groups quads, circles, triangles, lines, polygons, and textured sprites, using dynamic VAO/VBO/EBO, up to 16 simultaneous texture slots, plus a reserved `TextureArray` sampler unit for atlas-backed sprites. A per-object `IMaterial` can override the default shader for its own draws.
- **Object + component system**: `Object` as the base entity with a lifecycle (`Update`/`Draw`) and a type-safe `ComponentStorage` (based on `std::type_index`).
- **`ObjectPool`**: an object pool with a dense array + sparse map and `ObjectID` handles, designed for O(1) add/remove (swap-and-pop) without invalidating external references.
- **Layered render queue** (`RenderEntry`): support for sorting draws by `layer` (then by shader), avoiding unnecessary full re-sorts every frame.
- **Atlas + TextureArray asset pipeline**: `AssetsManager` loads sprite sheets into a GPU `TextureArray` and parses their layout from a JSON atlas file (`AtlasData`/`ParseAtlasJSON`) into either named `animations` (frame strips, for `IAnimator`) or named `tiles` (single-cell lookups, for tilemaps and static atlas sprites).
- **Animation**: `IAnimator` advances an atlas-backed `ISprite`'s frame over time (speed/step-controlled), driven by named animation regions from the atlas JSON.
- **Tile maps**: `TileMap` (`Engine/Map/TileMap`) loads a tile grid + a tile-atlas JSON, resolving each cell to a UV once at load time; `Scene::AddTileMap`/`RenderTileMaps` let a scene keep tile maps sorted by layer and interleave their draws with the object render queue (e.g. background tiles before the player, foreground tiles after).
- **Scene system**: a `Scene` interface (`Init`/`Update`/`Inputs`/`Render`) managed by a `ScenesManager`, each owning its own `Camera2D` and tile maps.
- **Service Locator** (`Services`): controlled global access.
- **Application layer** (`GameLayer`): decouples the engine from the actual game; the current executable (`Game`).
- **Asset loading**: `stb_image`, `stb_image_write`, and `stb_truetype` used for textures and, eventually, fonts.
- **Early serialization**: `Object::Save`/`Load` via `ObjectState`, the base for a future world-saving system (`WorldSaver`).

## Where it's headed

The mid-term goal is a complete, polished 2D engine. Next up:

- 2D collision/physics built on `IBoundingBox` (broad-phase + resolution).
- Expanding `TileMap` (collision layers, multi-atlas maps, editor-friendly authoring).
- Audio `IAudio` or a service to manage Audio and Music (`WAV`, `MP3`, `FLAC`, `OGG`).
- Remappable input (beyond the current `PollEvent` polling).
- Editor or support tooling (object inspection, asset hot-reload).
- Improving the world save/load system started in `WorldSaver`.

## Tech stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Rendering | OpenGL 3.3 Core |
| Window / Input / Platform | SDL3 (fetched via `FetchContent`) |
| GL function loading | GLAD (vendored in `external/glad`) |
| Images / fonts | stb_image, stb_image_write, stb_truetype |
| Math | GLM |
| Build system | CMake 3.25+ (Ninja Multi-Config) |

## Repository structure

```
Project_AK/
├── assets/              # Game resources (sprite atlases, tile maps, etc.)
│   └── Atlas/           # Atlas textures + JSON layouts (animations and/or tiles)
├── external/glad/       # Vendored OpenGL loader
├── shaders/             # GLSL shaders (texture.vs / texture.fs)
├── src/
│   ├── Main/            # Entry point (main.cpp)
│   ├── Engine/          # The engine itself
│   │   ├── Component/   # Components and storage (ECS-like)
│   │   ├── Object/      # Object and ObjectPool
│   │   ├── Layer/       # GameLayer and Scene (base interfaces)
│   │   ├── Map/         # TileMap: tile-grid loading and rendering
│   │   ├── Render/      # Batcher, Shader, Image, AtlasData, TextureArray, Color, Vertex...
│   │   ├── Services/    # AssetsManager, ScenesManager, WorldSaver
│   │   ├── PollEvent/   # Event/input handling
│   │   ├── Utils/       # Vector2, Rects, Config, Log, Path
│   │   └── Extern/      # Vendored third-party headers (stb)
│   └── Game/            # Game layer on top of the engine
│       ├── Scenes/      # MainMenu, Demo
│       └── Systems/
├── CMakeLists.txt
├── runner.sh            # Quick build + run
└── commands.cmd         # Reference CMake commands
```

## Building

### Requirements

- CMake 3.25 or higher
- Ninja
- A C++20-capable compiler (GCC/Clang)
- GLM installed on the system
- Drivers with OpenGL 3.3+ support
- (Linux) X11 development headers

SDL3 is downloaded and built automatically via `FetchContent`, so there's no need to install it separately.

### Steps

```bash
mkdir build && cd build
cmake -G "Ninja Multi-Config" ..
cd ..
cmake --build build --config Debug --target Project_AK
./build/Debug/Project_AK
```

Or, more directly, using the included script:

```bash
./runner.sh
```

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.
