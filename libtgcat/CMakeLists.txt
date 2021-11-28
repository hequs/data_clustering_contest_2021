cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(tgcat VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(TGCAT_CXX_FLAGS "-pthread;-Wall;-Wextra;-Wno-sign-compare;-fno-omit-frame-pointer")
set(TGCAT_CXX_DEBUG_FLAGS "-g;-O2;-fsanitize=address")
set(TGCAT_LNK_DEBUG_FLAGS "-fsanitize=address")
set(TGCAT_CXX_RELEASE_FLAGS "-funroll-loops;-Ofast;-march=x86-64;-mtune=generic;-msse3;-mssse3;-msse4;-msse4.1;-msse4.2")

find_package(ICU COMPONENTS uc REQUIRED)
find_package(Boost COMPONENTS regex REQUIRED)

set(INCLUDE_DIRS
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/thirdparty/fasttext/src
    ${CMAKE_SOURCE_DIR}/thirdparty/tokenizer/include
    ${ICU_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)

set(LINK_LIBS
    fasttext-static_pic
    OpenNMTTokenizer
    ${ICU_LIBRARIES}
    ${Boost_LIBRARIES}
)

set(SOURCE_FILES
    src/language_detector.cpp
    src/tgcat.cpp
    src/text_preprocessing.cpp
    src/topic_detector.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})
target_include_directories(${PROJECT_NAME} ${INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PUBLIC "${TGCAT_CXX_FLAGS}")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${TGCAT_CXX_DEBUG_FLAGS}>")
target_link_libraries(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${TGCAT_LNK_DEBUG_FLAGS}>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:${TGCAT_CXX_RELEASE_FLAGS}>")
