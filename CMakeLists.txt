cmake_minimum_required(VERSION 3.22.1)

project(d-color-extractor)

add_executable(${PROJECT_NAME} source/main.c source/color.c source/image_analyzer.c source/image_readers/jpeg.c source/models/histogram.c source/helpers.c)

target_link_libraries(${PROJECT_NAME} PUBLIC m turbojpeg)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Werror -Wpedantic -ggdb)
