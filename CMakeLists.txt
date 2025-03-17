cmake_minimum_required(VERSION 3.10.0)
project(LearnOpenGl VERSION 0.1.0 LANGUAGES C CXX)

 #   set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/spdlog/lib/cmake/spdlog/")
 #   find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/glad/include)
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

include_directories(${CMAKE_SOURCE_DIR}/spdlog/include)

add_executable(LearnOpenGl main.cpp practice.cpp)

target_precompile_headers(LearnOpenGl PRIVATE pch.h)

#target_link_libraries(LearnOpenGl  -lglfw glad -ldl spdlog::spdlog)
target_link_libraries(LearnOpenGl  -lglfw glad -ldl)