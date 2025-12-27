cmake_minimum_required(VERSION 3.20)
project(NonEuclideanChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
find_package(Threads REQUIRED)

set(CHESS_SOURCES
    src/common/BoardPosition.cpp
    src/common/Move.cpp
    src/board/Cell.cpp
    src/board/PortalLink.cpp
    src/board/GameBoard.cpp
    src/board/surfaces/Surface.cpp
    src/board/surfaces/RectangularSurface.cpp
    src/board/surfaces/CylindricalSurface.cpp
    src/board/surfaces/SphericalSurface.cpp
    src/pieces/ChessPiece.cpp
    src/pieces/King.cpp
    src/pieces/Queen.cpp
    src/pieces/Bishop.cpp
    src/pieces/Knight.cpp
    src/pieces/Rook.cpp
    src/pieces/Pawn.cpp
    src/game/Player.cpp
    src/game/ChessClock.cpp
    src/game/GameHistory.cpp
    src/game/GameManager.cpp
    src/utils/ThreadedVerdictCalculator.cpp
)

add_library(chess_lib STATIC ${CHESS_SOURCES})

option(COVERAGE "Enable coverage" OFF)

if(COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
    target_compile_options(chess_lib PRIVATE --coverage -O0 -g)
    target_link_options(chess_lib PRIVATE --coverage)
    message(STATUS "Coverage enabled (use gcov/llvm-cov)")
endif()

enable_testing()
find_package(GTest REQUIRED)

message(STATUS "Grid2D Container - Tests")

add_executable(test_grid2d tests/test_surface_grid.cpp)
target_link_libraries(test_grid2d chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME Grid2DTests COMMAND test_grid2d)

add_executable(test_cylindrical tests/test_cylindrical.cpp)
target_link_libraries(test_cylindrical chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME CylindricalTests COMMAND test_cylindrical)

add_executable(test_iterator_detailed tests/test_iterator_detailed.cpp)
target_link_libraries(test_iterator_detailed chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME IteratorDetailedTests COMMAND test_iterator_detailed)

add_executable(test_move_validation tests/test_move_validation.cpp)
target_link_libraries(test_move_validation chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME MoveValidationTests COMMAND test_move_validation)

add_executable(test_noneuclidean_surfaces tests/test_noneuclidean_surfaces.cpp)
target_link_libraries(test_noneuclidean_surfaces chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME NonEuclideanSurfaceTests COMMAND test_noneuclidean_surfaces)

add_executable(test_verdict_calculator tests/test_verdict_calculator.cpp)
target_link_libraries(test_verdict_calculator chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME VerdictCalculatorTests COMMAND test_verdict_calculator)

add_executable(test_hypothetical_check tests/test_hypothetical_check.cpp)
target_link_libraries(test_hypothetical_check chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME HypotheticalCheckTests COMMAND test_hypothetical_check)

add_executable(grid_example examples/grid_example.cpp)
target_link_libraries(grid_example chess_lib Threads::Threads)

add_executable(validation_test examples/validation_test.cpp)
target_link_libraries(validation_test chess_lib Threads::Threads)

add_executable(cylindrical_chess_game src/main.cpp)
target_link_libraries(cylindrical_chess_game chess_lib Threads::Threads)

add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_grid2d test_cylindrical test_iterator_detailed test_move_validation test_noneuclidean_surfaces test_verdict_calculator test_hypothetical_check
    COMMENT "Running all tests..."
)

add_custom_target(run_example
    COMMAND grid_example
    DEPENDS grid_example
    COMMENT "Running example..."
)

if(COVERAGE)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND echo "Generating coverage files..."
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -o -name "*.gcno"
        DEPENDS run_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests and generating coverage data"
    )

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if(LCOV AND GENHTML)
        add_custom_target(coverage_html
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND echo "Collecting coverage data..."
            COMMAND ${LCOV} --capture --directory . --output-file coverage_raw.info --rc derive_function_end_line=0 --ignore-errors format,inconsistent
            COMMAND echo "Filtering coverage data..."
            COMMAND ${LCOV} --remove coverage_raw.info '/usr/*' '/Library/*' '*/tests/*' '*/examples/*' '*/gtest/*' '*/googletest/*' '*/.brew/*' '*/c++/*' '*/bits/*' --output-file coverage_filtered.info --rc derive_function_end_line=0 --ignore-errors format,inconsistent,unused
            COMMAND echo "Generating HTML report..."
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_html --demangle-cpp --legend --title "Non-Euclidean Chess Coverage" --rc derive_function_end_line=0 --ignore-errors source,inconsistent,category
            COMMAND echo "========================================="
            COMMAND echo "Coverage report: coverage_html/index.html"
            COMMAND echo "========================================="
            DEPENDS run_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating HTML coverage report"
        )
        message(STATUS "Coverage targets available:")
        message(STATUS "  make coverage       - Generate .gcov files")
        message(STATUS "  make coverage_html  - Generate HTML report")
    else()
        message(STATUS "Coverage target available: make coverage")
        message(STATUS "Install lcov for HTML reports: brew install lcov")
    endif()
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "generating doc"
        VERBATIM
    )

    message(STATUS "Doxygen found - use 'make docs' to generate documentation")
    message(STATUS "Documentation will be in: build/docs/html/index.html")
else()
    message(WARNING "Doxygen not found - documentation generation disabled")
endif()

message(STATUS "Targets: make, make run_tests, ctest")

