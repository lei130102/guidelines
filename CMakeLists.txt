cmake_minimum_required(VERSION 3.9)

set(VCPKG_CMAKE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-windows")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE})

project(guidelines)

#不应该在这里写这些
#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CONFIGURATION_TYPES "Debug")

set(CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake/CMakeModules;$ENV{VCPKG_ROOT};${CMAKE_MODULE_PATH}")
set(CMAKE_PREFIX_PATH "$ENV{QTVCPKG};$ENV{VCPKG_ROOT};${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

set(BUILD_RUNTIME_DIR "bin")
set(BUILD_LIBRARY_DIR "lib")
set(BUILD_ARCHIVE_DIR "lib")

######

set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR}/src)
set(BINARY_DIR ${${PROJECT_NAME}_BINARY_DIR}/src)

#仍在测试的代码
set(TEST_SOURCE_DIR ${SOURCE_DIR}/test)
set(TEST_BINARY_DIR ${BINARY_DIR}/test)

#不同的目录对应不同级别的第三方依赖，这样方便在工程中添加移除目录，boost库和qt库是必有的

######

message(STATUS "CMAKE_TOOLCHAIN_FILE              =${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_INSTALL_PREFIX              =${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_MODULE_PATH                 =${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_PREFIX_PATH                 =${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_BUILD_TYPE                  =${CMAKE_BUILD_TYPE}")
message(STATUS "----------------------------------------------------------")
message(STATUS "SOURCE_DIR                        =${SOURCE_DIR}")
message(STATUS "BINARY_DIR                        =${BINARY_DIR}")
message(STATUS "TEST_SOURCE_DIR                   =${TEST_SOURCE_DIR}")
message(STATUS "TEST_BINARY_DIR                   =${TEST_BINARY_DIR}")

#####

add_subdirectory(${TEST_SOURCE_DIR})
