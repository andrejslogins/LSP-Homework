cmake_minimum_required(VERSION 3.8)
project(MD3)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c)
add_executable(MD3 ${SOURCE_FILES})
target_link_libraries(MD3 crypto)
