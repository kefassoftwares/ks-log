cmake_minimum_required(VERSION 2.8)
project (Log)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

if (WIN32)
add_definitions (-D__WIN__ -D_CRT_SECURE_NO_WARNINGS)
set(BOOST_ROOT "C:/Program Files/boost/boost_1_59_0")
set(BOOST_LIBRARYDIR "C:/Program Files/boost/boost_1_59_0/stage/lib")
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
endif (WIN32)

if (UNIX)
set(MY_GCC_COMPILE_FLAGS "-std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MY_GCC_COMPILE_FLAGS}" )
add_definitions (-D__UNIX__)
endif (UNIX)

find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories (${BOOST_ROOT})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)