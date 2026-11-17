file(GLOB gl_cpp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(Project_AK PRIVATE ${gl_cpp})

add_subdirectory(TileMap)
