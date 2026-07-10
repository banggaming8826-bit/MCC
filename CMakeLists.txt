cmake_minimum_required(VERSION 3.16)

project(MCC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MCC
    mcc.cpp
)

if (MSVC)
    target_compile_options(MCC PRIVATE /W4)
else()
    target_compile_options(MCC PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O3
    )
endif()
