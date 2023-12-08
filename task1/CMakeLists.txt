cmake_minimum_required(VERSION 3.5.1)
project(task1)

set(SRC src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")

