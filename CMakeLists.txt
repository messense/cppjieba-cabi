cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(cjieba)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_MACOSX_RPATH 1)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${PROJECT_SOURCE_DIR}/cppjieba/deps)
include_directories(${PROJECT_SOURCE_DIR}/cppjieba/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/lib/jieba.cc)

add_library(objlib OBJECT ${SOURCES})

add_library(cjieba SHARED $<TARGET_OBJECTS:objlib>)
add_library(cjieba_static STATIC $<TARGET_OBJECTS:objlib>)

set_target_properties(cjieba PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/jieba.h)

add_executable(demo ${PROJECT_SOURCE_DIR}/demo.c)
target_include_directories(demo PRIVATE ${PROJECT_SOURCE_DIR}/cppjieba/deps ${PROJECT_SOURCE_DIR}/cppjieba/include ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(demo m cjieba_static)
