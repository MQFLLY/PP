cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

add_executable(PopulationProtocol
    src/main.cpp
    src/CompleteGraph.cpp
    src/StringProtocol.cpp
    src/StringState.cpp
)

# 启用更严格的编译选项
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(PopulationProtocol PRIVATE -Wall -Wextra -pedantic)
endif()