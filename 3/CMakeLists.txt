cmake_minimum_required(VERSION 3.20)
project(NonEuclideanChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
find_package(Threads REQUIRED)

option(COVERAGE "Enable coverage" OFF)

if(COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    add_compile_options(--coverage)
    add_link_options(--coverage)
    message(STATUS "Coverage enabled (use gcov/llvm-cov)")
endif()

enable_testing()
find_package(GTest REQUIRED)

message(STATUS "Grid2D Container - Tests")

add_executable(test_grid2d tests/test_surface_grid.cpp)
target_link_libraries(test_grid2d GTest::GTest GTest::Main Threads::Threads)
add_test(NAME Grid2DTests COMMAND test_grid2d)

add_executable(test_cylindrical tests/test_cylindrical.cpp)
target_link_libraries(test_cylindrical GTest::GTest GTest::Main Threads::Threads)
add_test(NAME CylindricalTests COMMAND test_cylindrical)

add_executable(test_iterator_detailed tests/test_iterator_detailed.cpp)
target_link_libraries(test_iterator_detailed GTest::GTest GTest::Main Threads::Threads)
add_test(NAME IteratorDetailedTests COMMAND test_iterator_detailed)

add_executable(grid_example examples/grid_example.cpp)
target_link_libraries(grid_example Threads::Threads)

add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_grid2d test_cylindrical test_iterator_detailed
    COMMENT "Running all tests..."
)

add_custom_target(run_example
    COMMAND grid_example
    DEPENDS grid_example
    COMMENT "Running example..."
)


message(STATUS "Targets: make, make run_tests, ctest")

