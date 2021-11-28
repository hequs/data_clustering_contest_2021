cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(tgcat)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fasttext EXCLUDE_FROM_ALL)

set(LIB_ONLY True)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tokenizer EXCLUDE_FROM_ALL)
unset(LIB_ONLY)

add_subdirectory(libtgcat)
add_subdirectory(libtgcat-tester)

option(BUILD_TOOLS "Build the tools" OFF)
if(BUILD_TOOLS)
    add_subdirectory(tools/text_preprocessing)
endif()

option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest EXCLUDE_FROM_ALL)

    enable_testing()
    add_subdirectory(tests)
endif()
