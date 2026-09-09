# Project_AK — Engine Changelog

Compact record of engine changes and versions.

`Version.. | feature, bug, change > Descripción`

---

## Alpha 0.04b 

`Alpha 0.04b | update > update data/, now having something.`

`Alpha 0.04b | adding > THIRD_PARTY_NOTICIES, for libraries copyrigth`

## Alpha 0.04 — OpenGL 4.6

`Alpha 0.04 | change > Migrated the OpenGL context from OpenGL 3.3 Core to OpenGL 4.6 Core.`

`Alpha 0.04 | change > Updated GLAD to target OpenGL 4.6 Core.`

`Alpha 0.04 | change > Updated shaders from GLSL 330 to GLSL 460 while initially preserving the existing renderer logic.`

`Alpha 0.04 | change > Kept the existing renderer compatible with traditional OpenGL APIs, allowing a gradual migration toward modern OpenGL 4.x features.`

---

## Alpha 0.03

`Alpha 0.03 | feature > Implementación inicial del sistema de serialización del motor mediante ObjectState y WorldSaver.`

`Alpha 0.03 | feature > Implementación de elementos iniciales de GUI, incluyendo Button y Label.`

`Alpha 0.03 | feature > Integración de soporte para fuentes y recursos de texto, incluyendo stb_truetype y BakerFont.`

`Alpha 0.03 | feature > Incorporación de recursos y configuración relacionados con idiomas y texto.`

`Alpha 0.03 | bug > Corrección de errores generales encontrados durante la integración de serialización y GUI.`

`Alpha 0.03 | change > Incorporación de AddressSanitizer en configuraciones Debug para detectar errores de memoria.`

---

## Alpha 0.02C

`Alpha 0.02C | feature > Added macOS-specific support to renderer and platform initialization.`

`Alpha 0.02C | change > Configured the OpenGL context for macOS using a forward-compatible Core Profile.`

`Alpha 0.02C | change > Updated the project documentation and README.`

---

## Alpha 0.02B

`Alpha 0.02B | feature > Implemented the TileMap system for loading and rendering tile maps.`

`Alpha 0.02B | feature > Integrated tile atlases with the TileMap system.`

`Alpha 0.02B | feature > Implemented the 2D collision system based on IBoundingBox.`

`Alpha 0.02B | feature > Implemented solid collision resolution using the axis of least penetration.`

`Alpha 0.02B | feature > Implemented TriggerEvent to support interaction zones without physical collision resolution.`

`Alpha 0.02B | change > Integrated TileMap rendering with the layer system and render queue.`

---

## Alpha 0.02A

`Alpha 0.02A | feature > Implemented IAnimator for controlling sprite animations.`

`Alpha 0.02A | feature > Implemented AtlasData for loading and querying atlas information from JSON.`

`Alpha 0.02A | feature > Implemented TextureArray for storing atlas sprites on the GPU.`

`Alpha 0.02A | feature > Integrated TextureArray with the batched renderer.`

`Alpha 0.02A | feature > Added support for up to 16 texture slots in the batcher.`

`Alpha 0.02A | feature > Implemented IMaterial, allowing objects to use custom shaders and materials.`

`Alpha 0.02A | change > Extended the asset pipeline to support sprites, atlases, animations, and TextureArray.`

---

## Alpha 0.01Df

`Alpha 0.01Df | change > Added build and runtime support for Windows using MSVC.`

`Alpha 0.01Df | change > Adjusted the build system to improve portability across compilers.`

---

## Alpha 0.01De

`Alpha 0.01De | bug > Fixed issues related to building and distributing the project through GitHub.`

`Alpha 0.01De | change > Adjusted CMake and dependencies to avoid unnecessary GLM builds.`

---

## Alpha 0.01Dd

`Alpha 0.01Dd | bug > Fixed platform-specific includes and dependencies.`

`Alpha 0.01Dd | change > Replaced non-portable internal dependencies with appropriate C++/GLM includes.`

`Alpha 0.01Dd | change > Refactored EngineConfig into a class better suited to the engine architecture.`

`Alpha 0.01Dd | change > Fixed numeric literals to explicitly use float values.`

---

## Alpha 0.01Dc

`Alpha 0.01Dc | bug > Fixed additional issues required for building and publishing the project on GitHub.`

---

## Alpha 0.01Db

`Alpha 0.01Db | change > Added nlohmann/json as a project dependency.`

`Alpha 0.01Db | bug > Fixed build and runtime issues related to Windows 11.`

`Alpha 0.01Db | change > Adjusted dependencies and configuration to improve portability.`

---

## Alpha 0.01Db — GitHub Actions

`Alpha 0.01Db | change > Added and fixed the GitHub Actions pipeline for automated builds.`

`Alpha 0.01Db | bug > Fixed configuration issues required for GitHub Actions CI workflows to run correctly.`

---

## Alpha 0.01Da

`Alpha 0.01Da | change > Improved engine stability and prepared the codebase for subsequent development stages.`

---

## Alpha 0.01D

`Alpha 0.01D | feature > Implemented MusicManager for music playback using SDL3_mixer.`

`Alpha 0.01D | feature > Implemented SFXManager for sound effect management.`

`Alpha 0.01D | feature > Added support for simultaneous sound effects through audio pooling.`

`Alpha 0.01D | feature > Added per-resource audio volume control.`

`Alpha 0.01D | change > Moved ObjectPool ownership to Scene instead of GameLayer.`

`Alpha 0.01D | change > Improved separation between the application layer and scene-managed resources.`

---

## Alpha 0.01C

`Alpha 0.01C | change > Optimized GameApplication and the engine main execution loop.`

`Alpha 0.01C | change > Improved separation between the application layer and the engine.`

---

## Alpha 0.01B

`Alpha 0.01B | feature > Implemented ComponentStorage using std::type_index for type-safe component storage.`

`Alpha 0.01B | feature > Implemented ObjectID handles for identifying objects within the engine.`

`Alpha 0.01B | feature > Implemented ObjectPool using dense storage and a sparse map.`

`Alpha 0.01B | change > Optimized object insertion and removal using swap-and-pop.`

`Alpha 0.01B | feature > Added Systems to the Game layer.`

`Alpha 0.01B | change > Improved the object processing and rendering lifecycle.`

---

## Initial

`Alpha 0.01A | feature > Initial creation of Project_AK as a 2D game engine built with C++20.`

`Alpha 0.01A | feature > Implemented windowing, input, and graphics context management using SDL3.`

`Alpha 0.01A | feature > Implemented the initial renderer using OpenGL 3.3 Core.`

`Alpha 0.01A | feature > Implemented the initial batching system for 2D geometry.`

`Alpha 0.01A | feature > Implemented objects, components, scenes, and the GameLayer architecture.`

`Alpha 0.01A | feature > Added the initial texture, shader, 2D camera, and math utility systems.`

---
