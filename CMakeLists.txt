cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(mtasr VERSION 0.1)

option(GRPC "whether to build with gRPC" ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

include(FetchContent)
include(ExternalProject)
set(FETCHCONTENT_QUIET OFF)
get_filename_component(fc_base "fc_base" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(FETCHCONTENT_BASE_DIR ${fc_base})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(NOT MSVC)
  # Keep the same with openfst, -fPIC or -fpic
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -fPIC")
else()
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# Include all dependency
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Build all libraries
add_subdirectory(utils)
add_subdirectory(frontend)

include(grpc)
add_subdirectory(grpc)

# Build all bins
add_subdirectory(bin)
