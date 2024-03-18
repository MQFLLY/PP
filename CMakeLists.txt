cmake_minimum_required(VERSION 3.22.1)

project(PP)

set(SOURCE_FILES
    main.cpp
    src/timer/population_protocol.cpp
    src/timer/utils.cpp
    src/timer/population_protocol_simtime_ourwork.cpp
    src/timer/population_protocol_simtime_otherwork.cpp
    src/timer/population_protocol_simtime.cpp
)

add_executable(main_cmake ${SOURCE_FILES})

target_include_directories(main_cmake PRIVATE src/include)