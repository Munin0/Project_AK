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
## | ------------------------------------------------- |
### Making libraries available for the project
message("-- Making libraries available:")
message("--   - « SDL3 »")
FetchContent_MakeAvailable(SDL3)
### | ------------------------------------------------- |
add_executable(Project_AK)

add_subdirectory(src)

message("-- Libraries linked: ")
message("--   - « SDL3 »")
message("--   - « SDL3 Image »")
message("-- ")

target_compile_definitions(Project_AK PRIVATE 
      ASSET_PATH="${CMAKE_SOURCE_DIR}/assets/"
)
target_compile_definitions(Project_AK PRIVATE 
      CONFIG_PATH="${CMAKE_SOURCE_DIR}/config/"
)
target_compile_definitions(Project_AK PRIVATE 
      SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/"
)


target_link_libraries(Project_AK PRIVATE SDL3::SDL3)
