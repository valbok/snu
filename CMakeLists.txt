cmake_minimum_required(VERSION 2.6)

project(snu)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(lib/gtest)
include_directories(lib/gtest/include)

add_library(gtest
    lib/gtest/src/gtest_main.cc 
    lib/gtest/src/gtest-all.cc)

add_subdirectory(lib/enki)

add_subdirectory(snu/brain)
add_subdirectory(snu/brain/tests)
add_subdirectory(snu/worm)
add_subdirectory(snu/worm/tests)
add_subdirectory(snu/robots/e-puck)
add_subdirectory(snu/robots/e-puck/tests)
add_subdirectory(viewer)
