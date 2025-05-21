cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. 首先引入 fmt 库（spdlog 的依赖）
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

# 2. 配置 spdlog 使用外部 fmt
option(SPDLOG_FMT_EXTERNAL ON)
option(SPDLOG_BUILD_EXAMPLE OFF)
option(SPDLOG_BUILD_TESTS OFF)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# 3. 配置 gflags
find_package(gflags QUIET)
if(NOT gflags_FOUND)
    message(STATUS "gflags not found, building from source")
    FetchContent_Declare(
        gflags_src
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG v2.2.2
    )
    FetchContent_MakeAvailable(gflags_src)
    set(gflags_FOUND TRUE)
    add_library(gflags::gflags ALIAS gflags)
endif()

# 4. DuckDB 配置
add_subdirectory(thirdparty/duckdb)

# 可执行目标
add_executable(PopulationProtocol
    src/main.cpp
    src/graph/CompleteGraph.cpp
    src/protocol/StringProtocol.cpp
    src/state/StringState.cpp
)

# 依赖链接（注意顺序）
target_link_libraries(PopulationProtocol PRIVATE 
    duckdb
    spdlog::spdlog
    fmt::fmt
    gflags
)

# 包含目录
target_include_directories(PopulationProtocol PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/duckdb/src/include
)

# 编译选项
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