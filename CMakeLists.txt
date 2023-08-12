cmake_minimum_required(VERSION 3.14)

project(openGL-tutorial)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3-stable
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable( glfw )


FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
FetchContent_MakeAvailable( glad )

include_directories(include)
add_executable(game src/main.cpp)
target_link_libraries(game glfw glad)

# Copying the shaders into the build folder

# file(GLOB shaders ${CMAKE_SOURCE_DIR}/src/glsl/*)

# foreach(shader ${shaders})
# # file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/glsl/ INPUT ${shader})
# configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/glsl/ COPYONLY)
# endforeach(shader)

add_custom_target(copy_glsl
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/glsl/ ${CMAKE_CURRENT_BINARY_DIR}/glsl/
)
add_dependencies(game copy_glsl)

# add_custom_command(TARGET game POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#   ${CMAKE_SOURCE_DIR}/src/glsl/
#   $<TARGET_FILE_DIR:game>/glsl/
#   )

# Copy the assets folder into the build directory
set(link_src ${CMAKE_SOURCE_DIR}/assets)
set(link_dst ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${link_src} ${link_dst}
)
add_dependencies(game copy_assets)