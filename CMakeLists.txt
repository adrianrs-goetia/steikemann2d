cmake_minimum_required(VERSION 3.28)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(
    Steikemann2D
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES CXX C
)

add_subdirectory(src)
