cmake_minimum_required(VERSION 3.14)
project(MancalaEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Include FetchContent for header-only third party libraries
include(FetchContent)

# Fetch nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Fetch cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.15.3
)
FetchContent_MakeAvailable(httplib)

# Define library source files
set(ENGINE_LIB_SOURCES
    src/board.h
    src/board.cpp
    src/heuristic.h
    src/heuristic.cpp
    src/minimax.h
    src/minimax.cpp
    src/alphabeta.h
    src/alphabeta.cpp
    src/parallel.h
    src/parallel.cpp
)

# Shared library/object target for engine logic
add_library(engine_lib STATIC ${ENGINE_LIB_SOURCES})
target_link_libraries(engine_lib PRIVATE OpenMP::OpenMP_CXX)

# Main Executable (HTTP Server)
add_executable(mancala_engine src/main.cpp src/server.cpp)
target_link_libraries(mancala_engine PRIVATE engine_lib nlohmann_json::nlohmann_json httplib::httplib OpenMP::OpenMP_CXX)

# Benchmark Executable
add_executable(benchmark_main bench/benchmark_main.cpp)
target_link_libraries(benchmark_main PRIVATE engine_lib OpenMP::OpenMP_CXX)

# Enable Testing
enable_testing()

# Helper macro to register tests
macro(add_mancala_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE engine_lib OpenMP::OpenMP_CXX)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

# Register Tests
add_mancala_test(test_board tests/test_board.cpp)
add_mancala_test(test_capture tests/test_capture.cpp)
add_mancala_test(test_extra_turn tests/test_extra_turn.cpp)
add_mancala_test(test_endgame tests/test_endgame.cpp)
add_mancala_test(test_minimax tests/test_minimax.cpp)
add_mancala_test(test_alphabeta tests/test_alphabeta.cpp)
add_mancala_test(test_equivalence tests/test_equivalence.cpp)
