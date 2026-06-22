cmake_minimum_required(VERSION 3.25)
  project(Project_AK LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
### | ------------------------------------------------- |
### SDL3 Include && Dowload
message("-- Dowloading SDL3 Library")
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.4
)
### Confi for SDL3
set(SDL3IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_PNG ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_JPG ON CACHE BOOL "" FORCE)
### SDL3_Mixer 
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.4
)
set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
### | ------------------------------------------------- |
### GLM (header-only)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
### | ------------------------------------------------- |
### nlohmann_json Include
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
)
## | ------------------------------------------------- |
### Making libraries available for the project
message("-- Making libraries available:")
message("--   - « SDL3 »")
message("--   - « SDL3_Mixer »")
message("--   - « GLM »")
message("--   - « nlohmann_json »")
FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(SDL3_Mixer)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(json)
### | ------------------------------------------------- |
add_executable(Project_AK)

add_subdirectory(src)

message("-- Libraries linked: ")
message("--   - « SDL3 »")
message("--   - « SDL3 Image »")
message("--   - « GLM »")
message("--   - « nlohmann_json »")
message("-- ")

target_link_libraries(Project_AK PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer glm::glm nlohmann_json::nlohmann_json)

### | ------------------------------------------------- |
## Copy foldets to Debug
add_custom_command(TARGET Project_AK POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Project_AK>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Project_AK>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:Project_AK>/config
)
### | ------------------------------------------------- |
