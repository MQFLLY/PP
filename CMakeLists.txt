cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(PopulationProtocol
    src/main.cpp
    src/graph/CompleteGraph.cpp
    src/protocol/StringProtocol.cpp
    src/state/StringState.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(PopulationProtocol PRIVATE -Wall -Wextra -pedantic)
endif()