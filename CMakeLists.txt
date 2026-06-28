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
    src/board/surfaces/StandardSurface.cpp
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

enable_testing()
find_package(GTest REQUIRED)


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

add_executable(test_surfaces tests/test_surfaces.cpp)
target_link_libraries(test_surfaces chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME SurfaceTests COMMAND test_surfaces)

add_executable(test_standard_surface tests/test_standard_surface.cpp)
target_link_libraries(test_standard_surface chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME StandardSurfaceTests COMMAND test_standard_surface)

add_executable(test_portals tests/test_portals.cpp)
target_link_libraries(test_portals chess_lib GTest::GTest GTest::Main Threads::Threads)
add_test(NAME PortalTests COMMAND test_portals)

add_executable(cylindrical_chess_game src/main.cpp)
target_link_libraries(cylindrical_chess_game chess_lib Threads::Threads)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark chess_lib Threads::Threads)


add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_grid2d test_cylindrical test_iterator_detailed test_move_validation test_noneuclidean_surfaces test_verdict_calculator test_hypothetical_check test_surfaces test_portals test_standard_surface
)

add_custom_target(run_example
    COMMAND grid_example
    DEPENDS grid_example
)

add_custom_target(run_benchmark
    COMMAND benchmark
    DEPENDS benchmark
    COMMENT "Running benchmark..."
)

if(COVERAGE)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND echo "Generating coverage files..."
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -o -name "*.gcno"
        DEPENDS run_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if(LCOV AND GENHTML)
        add_custom_target(coverage_html
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV} --capture --directory . --output-file coverage_raw.info --rc derive_function_end_line=0 --ignore-errors format,inconsistent
            COMMAND ${LCOV} --remove coverage_raw.info '/usr/*' '/Library/*' '*/tests/*' '*/examples/*' '*/gtest/*' '*/googletest/*' '*/.brew/*' '*/c++/*' '*/bits/*' --output-file coverage_filtered.info --rc derive_function_end_line=0 --ignore-errors format,inconsistent,unused
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_html --demangle-cpp --legend --title "Non-Euclidean Chess Coverage" --rc derive_function_end_line=0 --ignore-errors source,inconsistent,category
            DEPENDS run_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
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
        VERBATIM
    )
endif()

add_custom_target(demo
    COMMAND ${CMAKE_COMMAND} -E echo "Running Non-Euclidean Chess Portal Demo..."
    COMMAND python3 ${CMAKE_SOURCE_DIR}/benchmarks/demo_portals.py
    DEPENDS cylindrical_chess_game
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

