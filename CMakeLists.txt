cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0  # 指定稳定版本
)
FetchContent_MakeAvailable(spdlog)

find_package(gflags QUIET)

if(NOT gflags_FOUND)
    message(STATUS "gflags not found, building from source")
    include(FetchContent)
    FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG v2.2.2
    )
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(gflags)
endif()

add_subdirectory(thirdparty/duckdb)

add_executable(PopulationProtocol
    src/main.cpp
    src/graph/CompleteGraph.cpp
    src/protocol/StringProtocol.cpp
    src/state/StringState.cpp
)

target_link_libraries(PopulationProtocol PRIVATE 
    duckdb
    spdlog::spdlog
    gflags
)

target_include_directories(PopulationProtocol PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/duckdb/src/include
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(PopulationProtocol PRIVATE 
        -Wall 
        -Wextra 
        -pedantic
        -DSPDLOG_COMPILED_LIB
        -DSPDLOG_FMT_EXTERNAL
    )
endif()

set_target_properties(PopulationProtocol PROPERTIES
    BUILD_RPATH_USE_ORIGIN TRUE
    INSTALL_RPATH "@executable_path/../lib"
)

install(TARGETS PopulationProtocol DESTINATION bin)