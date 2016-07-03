cmake_minimum_required(VERSION 2.8)
project (Log)


if (WIN32)
add_definitions (-D__WIN__ -D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

if (UNIX)
add_definitions (-D__UNIX__)
set(MY_GCC_COMPILE_FLAGS "-std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MY_GCC_COMPILE_FLAGS}" )
endif (UNIX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(kslog)
add_subdirectory(MergeLogsApp)