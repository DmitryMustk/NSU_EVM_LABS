cmake_minimum_required(VERSION 3.10)
project(task6)

set(exe_name ${PROJECT_NAME})
add_compile_options(-O1 -march=native )

add_executable(${exe_name} main.c)
