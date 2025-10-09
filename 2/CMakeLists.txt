cmake_minimum_required(VERSION 3.15)
project(WordArrayProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ВСЕГДА включаем отладочную информацию
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")  # -O0 для лучшей отладки
set(CMAKE_BUILD_TYPE Debug)

# Coverage флаги только в Coverage режиме
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Глобальные пути включения
include_directories(include)

# Библиотека
add_library(WordLibrary STATIC src/Word.cpp src/WordArray.cpp)
target_include_directories(WordLibrary PUBLIC include)

# Основные тесты Word
add_executable(WordTests tests/testWord.cpp)
target_link_libraries(WordTests WordLibrary)

# Тесты WordArray
add_executable(WordArrayTests tests/testWordArray.cpp)
target_link_libraries(WordArrayTests WordLibrary)

find_package(GTest REQUIRED)
target_link_libraries(WordTests GTest::gtest GTest::gtest_main)
target_link_libraries(WordArrayTests GTest::gtest GTest::gtest_main)

# ДОБАВЛЯЕМ CTest ПЕРЕД созданием кастомных целей
enable_testing()
add_test(NAME WordTests COMMAND WordTests)
add_test(NAME WordArrayTests COMMAND WordArrayTests)

# Основная цель - все тесты (ИЗМЕНЕНО ИМЯ)
add_custom_target(run-all-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS WordTests WordArrayTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests with CTest"
)

# Индивидуальные цели тестов
add_custom_target(run-word-tests 
    COMMAND ./WordTests
    DEPENDS WordTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Word tests"
)

add_custom_target(run-array-tests 
    COMMAND ./WordArrayTests
    DEPENDS WordArrayTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running WordArray tests"
)

# Valgrind для всех тестов
find_program(VALGRIND valgrind)
if(VALGRIND)
    # Valgrind для WordArray с упрощенными настройками
    add_custom_target(valgrind-array
        COMMAND ${VALGRIND} 
                --leak-check=full 
                --show-leak-kinds=definite,possible
                --track-origins=yes
                --errors-for-leak-kinds=definite,possible
                --error-exitcode=1 
                ./WordArrayTests
        DEPENDS WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running Valgrind memory check for WordArray"
    )
    
    # Valgrind только для Word
    add_custom_target(valgrind-word
        COMMAND ${VALGRIND} 
                --leak-check=full 
                --show-leak-kinds=definite,possible
                --track-origins=yes
                --error-exitcode=1 
                ./WordTests
        DEPENDS WordTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running Valgrind memory check for Word"
    )
    
    # Полная проверка всех тестов
    add_custom_target(valgrind-all
        COMMAND ${VALGRIND} 
                --leak-check=full 
                --show-leak-kinds=definite,possible
                --error-exitcode=1 
                ./WordTests
        COMMAND ${VALGRIND} 
                --leak-check=full 
                --show-leak-kinds=definite,possible
                --error-exitcode=1 
                ./WordArrayTests
        DEPENDS WordTests WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running Valgrind memory check for all tests"
    )
    
    # Упрощенная версия для быстрой проверки
    add_custom_target(valgrind-quick
        COMMAND ${VALGRIND} 
                --leak-check=summary 
                --error-exitcode=1 
                ./WordArrayTests
        DEPENDS WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running quick Valgrind check for WordArray"
    )
    
else()
    message(WARNING "Valgrind not found - memory check targets will not be available")
endif()

# Coverage цели (упрощенные)
find_program(GCOVR gcovr)
if(GCOVR)
    add_custom_target(coverage
        COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} 
                --exclude=".*test.*" 
                --html --html-details 
                --output=coverage.html
        COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} 
                --exclude=".*test.*" 
                --print-summary
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage report"
    )
endif()

# GDB отладка
find_program(GDB gdb)
if(GDB)
    add_custom_target(debug-array
        COMMAND ${GDB} -ex "run" -ex "bt" --args ./WordArrayTests
        DEPENDS WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Debugging WordArrayTests with GDB"
    )
endif()

# Очистка
add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning build"
)

# Информация о целях
add_custom_target(help-targets
    COMMAND echo "=== Available Targets ==="
    COMMAND echo "Testing:"
    COMMAND echo "  run-all-tests         - Run all tests with CTest"
    COMMAND echo "  run-array-tests       - Run only WordArray tests"
    COMMAND echo "  run-word-tests        - Run only Word tests"
    COMMAND echo "  test                  - CTest built-in target"
    COMMAND echo ""
    COMMAND echo "Memory Checking:"
    COMMAND echo "  valgrind-array        - Valgrind check for WordArray"
    COMMAND echo "  valgrind-word         - Valgrind check for Word" 
    COMMAND echo "  valgrind-all          - Valgrind check for all tests"
    COMMAND echo "  valgrind-quick        - Quick Valgrind check"
    COMMAND echo ""
    COMMAND echo "Debugging:"
    COMMAND echo "  debug-array           - Debug with GDB"
    COMMAND echo ""
    COMMAND echo "Coverage:"
    COMMAND echo "  coverage              - Generate coverage report"
    COMMAND echo ""
    COMMAND echo "Clean:"
    COMMAND echo "  clean-build           - Clean build"
)
