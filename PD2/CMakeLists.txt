cmake_minimum_required(VERSION 3.8)
project(PD2)

set(CMAKE_C_STANDARD 99)


set(SOURCE_FILES main.c)
add_executable(PD2 ${SOURCE_FILES})
include_directories(lib)
#add_library(main.c math.h)
target_link_libraries(PD2 m)
#add_custom_target(Makefile)