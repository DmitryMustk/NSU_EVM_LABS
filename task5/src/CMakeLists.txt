cmake_minimum_required(VERSION 3.10)
project(task5)

set(exe_name ${PROJECT_NAME})
add_compile_options(-Wall -Wextra -pedantic -Werror -O1 -march=native)
add_library(corelib OBJECT matrix.cpp)

add_executable(${exe_name} main.cpp)
target_link_libraries(${exe_name} corelib)

