cmake_minimum_required(VERSION 3.14)
project(PopulationProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用 FetchContent 模块
include(FetchContent)

# 方式1：使用 FetchContent 自动下载 spdlog（推荐）
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0  # 指定稳定版本
)
FetchContent_MakeAvailable(spdlog)

# 方式2：如果已手动安装 spdlog，直接链接
# find_package(spdlog REQUIRED)

# DuckDB 配置
add_subdirectory(thirdparty/duckdb)

# 可执行目标
add_executable(PopulationProtocol
    src/main.cpp
    src/graph/CompleteGraph.cpp
    src/protocol/StringProtocol.cpp
    src/state/StringState.cpp
)

# 依赖链接
target_link_libraries(PopulationProtocol PRIVATE 
    duckdb
    spdlog::spdlog  # 链接 spdlog
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
        # 可选：启用 spdlog 的编译期格式化检查
        -DSPDLOG_COMPILED_LIB
        -DSPDLOG_FMT_EXTERNAL
    )
endif()

# 配置目标属性
set_target_properties(PopulationProtocol PROPERTIES
    BUILD_RPATH_USE_ORIGIN TRUE
    INSTALL_RPATH "@executable_path/../lib"
)

# 可选：安装规则（如需打包）
install(TARGETS PopulationProtocol DESTINATION bin)