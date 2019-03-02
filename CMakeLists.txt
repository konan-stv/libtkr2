cmake_minimum_required(VERSION 3.13)

set (PROJECT_NAME "libtkr2")

project (${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Subversion)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(${PROJECT_NAME} VERSION 2.0.$ENV{TRAVIS_BUILD_NUMBER})
    set (ENV{TRAVIS_PROJECT_NAME} "${PROJECT_NAME}")
    set(file_sh ${CMAKE_CURRENT_BINARY_DIR}/pname.sh)
    file(WRITE ${file_sh} "#!/usr/bin/env bash\n")
    file(APPEND ${file_sh} "export TRAVIS_PROJECT_NAME=\"${PROJECT_NAME}\"")
    execute_process(COMMAND chmod a+x ${file_sh} RESULT_VARIABLE res)
elseif (Subversion_FOUND)
    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Prj)
    project(${PROJECT_NAME} VERSION 2.0.${Prj_WC_REVISION})
else()
    project(${PROJECT_NAME} VERSION 2.0.0)
endif()

file(WRITE svnversion.h "#define MINVERSION ${PROJECT_VERSION_MINOR}\n#define SVNVERSION ${PROJECT_VERSION_PATCH}\n")

set (GTEST_ROOT /usr/src/gtest)
set (GTEST_LIBRARY /usr/src/gtest/libgtest.a)
set (GTEST_MAIN_LIBRARY /usr/src/gtest/libgtest_main.a)

find_package (GTest REQUIRED)
find_package (Threads REQUIRED)
include_directories (${GTEST_INCLUDE_DIR})

FILE(GLOB SRCS "*.cpp")

add_library(tkr2 libtkr2.cpp libtkr2.h svnversion.h libtkr2_types.h libtkr2_core.h libtkr2_core.cpp)
add_executable(runTest test.cpp)
add_dependencies(runTest tkr2)

target_link_libraries(
        runTest ${GTEST_LIBRARIES} Threads::Threads tkr2)

set_target_properties(runTest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;--std=c++1z"
        )

enable_testing()

add_test(runTest runTest)