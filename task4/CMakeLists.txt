cmake_minimum_required(VERSION 3.10)

project(task4)

# Версия 1
if(VERSION STREQUAL "v1")
    message("Building v1 (reverse matrix without vectorization)")
    list(APPEND SOURCES
        src/reverse_matrix1.cpp
    )
endif()

# Версия 2
if(VERSION STREQUAL "v2")
    message("Building v2 (reverse matrix with asm includes)")
    list(APPEND SOURCES
        src/reverse_matrix2.cpp
    )
endif()

#Версия 3
if(VERSION STREQUAL "v3")
    message("Building Version 3 (reverse matrix with BLAS funcs)")
    find_library(CBLAS libblas.a /usr/local/lib)
    list(APPEND SOURCES
        src/reverse_matrix3.cpp
    )
endif()


add_executable(task4 ${SOURCES})
#target_compile_options(task4 PRIVATE -O3)

if(VERSION STREQUAL "v3")
    target_link_libraries(${PROJECT_NAME} ${CBLAS})
endif()