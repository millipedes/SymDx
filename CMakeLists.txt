cmake_minimum_required(VERSION 3.5)
project(sym_diff)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(SRC_FILES
  ${SRC_DIR}/parsing/parsing.cpp
  ${SRC_DIR}/parsing/parsing_ds.cpp
)

include_directories(
  ${SRC_DIR}
)

add_executable(slc ${SRC_FILES} src/main/main.cpp)

set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")
option(BUILD_TESTS "Build Test Suite" ON)
if(BUILD_TESTS)
  enable_testing()
  set(TEST_SRC_FILES
    ${TEST_DIR}/parsing.cpp
    ${TEST_DIR}/test_helper.cpp
  )
  add_executable(tests ${TEST_SRC_FILES} test/main.cpp)
  target_link_libraries(tests ${SRC_FILES} gtest gtest_main pthread)
endif()
