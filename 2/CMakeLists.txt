cmake_minimum_required(VERSION 3.15)
project(WordArrayProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

include_directories(include)

# 1. lib compily (static and shared)
#cmake -B build
#cmake --build build
# in result libWordLibraryStatic.a и libWordLibraryShared.so in build
add_library(WordLibraryStatic STATIC src/Word.cpp src/WordArray.cpp)
add_library(WordLibraryShared SHARED src/Word.cpp src/WordArray.cpp)

target_include_directories(WordLibraryStatic PUBLIC include)
target_include_directories(WordLibraryShared PUBLIC include)

# 2.  smt for test
add_executable(WordTests tests/testWord.cpp)
add_executable(WordArrayTests tests/testWordArray.cpp)

target_link_libraries(WordTests WordLibraryStatic)
target_link_libraries(WordArrayTests WordLibraryStatic)

find_package(GTest REQUIRED)
target_link_libraries(WordTests GTest::gtest GTest::gtest_main)
target_link_libraries(WordArrayTests GTest::gtest GTest::gtest_main)

# 3. valgrind - test
find_program(VALGRIND valgrind)
if(VALGRIND)
    add_custom_target(valgrind-all
        COMMAND ${VALGRIND} --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./WordTests
        COMMAND ${VALGRIND} --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./WordArrayTests
        DEPENDS WordTests WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_custom_target(valgrind-word
        COMMAND ${VALGRIND} --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./WordTests
        DEPENDS WordTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_custom_target(valgrind-array
        COMMAND ${VALGRIND} --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./WordArrayTests
        DEPENDS WordArrayTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

# 4. docs
find_program(DOXYGEN doxygen)
if(DOXYGEN)
    add_custom_target(docs
        COMMAND ${DOXYGEN} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
    )
endif()

# 5. more option tests
add_custom_target(run-tests
    COMMAND ./WordTests && ./WordArrayTests
    DEPENDS WordTests WordArrayTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests"
)

add_custom_target(run-word-tests
    COMMAND ./WordTests
    DEPENDS WordTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run-array-tests
    COMMAND ./WordArrayTests
    DEPENDS WordArrayTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)