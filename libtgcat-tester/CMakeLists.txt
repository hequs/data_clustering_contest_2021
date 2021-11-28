cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(tgcat-tester VERSION 2.0.0 LANGUAGES C)

add_executable(tgcat-tester tester.c json.c json-builder.c)

target_link_libraries(tgcat-tester PRIVATE tgcat)

if (NOT WIN32)
  target_link_libraries(tgcat-tester PRIVATE m)
  if (UNIX AND NOT APPLE)
    target_link_libraries(tgcat-tester PRIVATE rt)
  endif()
else()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
