cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(text_preprocessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(ICU COMPONENTS uc REQUIRED)
find_package(Threads REQUIRED)

set(INCLUDE_DIRS
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${ICU_INCLUDE_DIRS}
)

set(LINK_LIBS
    OpenNMTTokenizer
    ${ICU_LIBRARIES}
    tgcat
    Threads::Threads
)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(${PROJECT_NAME} ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
