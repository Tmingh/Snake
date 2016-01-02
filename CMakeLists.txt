cmake_minimum_required(VERSION 3.3)
project(new_snake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

link_libraries(SDL2 SDL2_image SDL2_ttf)
link_libraries(m)


include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})
add_definitions(${GTK2_CFLAGS_OTHER})

set(SOURCE_FILES snake_function.h snake_struct.h snake.c)
add_executable(new_snake ${SOURCE_FILES} snake_function.h snake_struct.h snake.c cJSON.c)
target_link_libraries(new_snake ${GTK2_LIBRARIES})