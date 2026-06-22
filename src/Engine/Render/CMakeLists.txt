add_subdirectory(Batching)
add_subdirectory(Camera)
add_subdirectory(Color)
add_subdirectory(Geometry)
add_subdirectory(Image)
add_subdirectory(Shaders)

file(GLOB_RECURSE gl_cpp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(Project_AK PRIVATE ${gl_cpp})
