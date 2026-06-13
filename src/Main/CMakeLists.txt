target_include_directories(Project_AK PRIVATE 
  ${PROJECT_SOURCE_DIR}/external/glad/include
)

target_sources(Project_AK PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c)
