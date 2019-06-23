cmake_minimum_required(VERSION 2.8.0)
project(libGrbl CXX)

find_package(GLM REQUIRED)

file(GLOB_RECURSE SRC_FILES "*.cpp" "*.h")

include_directories(
	${GLM_INCLUDE_DIRS}
)

if(WIN32)
    add_library(Grbl ${SRC_FILES)
else()
    add_library(Grbl SHARED ${SRC_FILES})
endif(WIN32)

set(${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
