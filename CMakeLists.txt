cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DUCKDB_LIB ${CMAKE_SOURCE_DIR}/../duckdb/build/release/src/libduckdb.dylib)
set(DUCKDB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../duckdb/src/include)

include_directories(include)

add_executable(PopulationProtocol
    src/main.cpp
    src/graph/CompleteGraph.cpp
    src/protocol/StringProtocol.cpp
    src/state/StringState.cpp
)

target_include_directories(PopulationProtocol PRIVATE
    ${DUCKDB_INCLUDE_DIR}
)
target_link_libraries(PopulationProtocol ${DUCKDB_LIB})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(PopulationProtocol PRIVATE -Wall -Wextra -pedantic)
endif()

set_target_properties(PopulationProtocol PROPERTIES
    BUILD_RPATH_USE_ORIGIN TRUE
    INSTALL_RPATH "@executable_path/../lib"
)