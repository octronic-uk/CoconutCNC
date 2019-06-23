cmake_minimum_required(VERSION 2.8.0)
project(CoconutCNC CXX)

# Set Cpp Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable In-Source Builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

# Makefile Settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  # BigObj is required by sol2
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
  message("MSVC Version: ${MSVC_VERSION} | Toolset: ${MSVC_TOOLSET_VERSION}")
endif()

# Directories
#add_subdirectory(libGrbl)
add_subdirectory(Client)
