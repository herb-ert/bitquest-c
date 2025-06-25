cmake_minimum_required(VERSION 3.31)
project(BitQuest C)
set(CMAKE_C_STANDARD 23)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/core
  ${CMAKE_SOURCE_DIR}/include/world
  ${CMAKE_SOURCE_DIR}/include/entities
  "C:/msys64/mingw64/include/SDL2"
)

link_directories("C:/msys64/mingw64/lib")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  src/*.c
  include/**/*.h
)

add_executable(BitQuest ${SOURCES})

target_link_libraries(BitQuest mingw32 SDL2main SDL2 SDL2_image)

add_custom_command(TARGET BitQuest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "C:/msys64/mingw64/bin/SDL2.dll"
  "C:/msys64/mingw64/bin/SDL2_image.dll"
  $<TARGET_FILE_DIR:BitQuest>
)