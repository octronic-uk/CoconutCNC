cmake_minimum_required(VERSION 2.8.0)
project(CoconutCNC C CXX)

# Compiler Variables ###########################################################

# Set Cpp Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CFlags
# -fms-exxtensions required for libserialport
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fdeclspec)

# Disable In-Source Builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

# Makefile Settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   OFF)

# Find Sources #################################################################

file(GLOB_RECURSE SRC_FILES	"src/*.h"        "src/*.cpp")
file(GLOB IMGUI_SRC_FILES   "Deps/ImGui/*.cpp" "Deps/ImGui/*.h")

set(GLAD_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Deps/glad/include")
set(GLAD_SRC           "${PROJECT_SOURCE_DIR}/Deps/glad/src/glad.c")
set(GLM_INCLUDE_DIR    "${PROJECT_SOURCE_DIR}/Deps/glm/")
set(IMGUI_INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/Deps/ImGui")
set(IFCH_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Deps/IconFontCppHelpers")
set(JSON_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Deps/nlohmann/")
set(NFD_INCLUDE_DIR    "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/src/include")
set(NFD_LIBRARY_MAC    "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/build/lib/Release/x64/libnfd.a")
set(NFD_LIBRARY_LINUX  "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/build/lib/Release/x64/libnfd.a")
set(NFD_LIBRARY_WIN    "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/build/lib/Release/x64/libnfd.dll")
set(SERIALPORT_DIR     "${PROJECT_SOURCE_DIR}/Deps/libserialport")
set(SPDLOG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/spdlog/include")

# for LibSerialPort
set(SERIALPORT_SRC_FILES
	"${SERIALPORT_DIR}/serialport.c"
	"${SERIALPORT_DIR}/libserialport_internal.h"
	"${SERIALPORT_DIR}/libserialport.h"
	"${SERIALPORT_DIR}/config.h"
)

if (WIN32)
	set (SERIALPORT_SRC_FILES ${SERIALPORT_SRC_FILES}	"${SERIALPORT_DIR}/windows.c")
endif(WIN32)

if (UNIX AND NOT APPLE)
	set (SERIALPORT_SRC_FILES
		${SERIALPORT_SRC_FILES}
		"${SERIALPORT_DIR}/linux.c"
		"${SERIALPORT_DIR}/linux_termios.c"
		"${SERIALPORT_DIR}/linux_termios.h"
	)
endif(UNIX AND NOT APPLE)

if(APPLE)
	set (SERIALPORT_SRC_FILES ${SERIALPORT_SRC_FILES} "${SERIALPORT_DIR}/macosx.c")
endif(APPLE)

# Find Dependencies ############################################################

find_package(Freetype REQUIRED)
find_package(glfw3    REQUIRED)
find_package(OpenGL   REQUIRED)

# Targets ######################################################################

add_executable(
	${PROJECT_NAME}
	${SRC_FILES}
	${IMGUI_SRC_FILES}
	${SERIALPORT_SRC_FILES}
	${GLAD_SRC}
)

include_directories(
	${Freetype_INCLUDE_DIRS}
	${glfw3_INCLUDE_DIR}
	${OpenGL_INCLUDE_DIR}
	# In /Deps
	${GLAD_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${IFCH_INCLUDE_DIR}
	${SERIALPORT_DIR}
	${NFD_INCLUDE_DIR}
	${JSON_INCLUDE_DIR}
	${SPDLOG_INCLUDE_DIR}
)


if (WIN32)
    target_link_libraries(
		${PROJECT_NAME}
		${GLFW3_LIBRARY}
		${Freetype_LIBRARY}
	)
# Linux
elseif (UNIX AND NOT APPLE)
    target_link_libraries(
		${PROJECT_NAME}
		-lpthread
		-ldl
		glfw
		${Freetype_LIBRARY}
		${OPENGL_LIBRARIES}
	)
elseif (APPLE)
	    target_link_libraries(
		${PROJECT_NAME}
		-lpthread
		-ldl
		"-framework IOKit"
		"-framework AppKit"
		"-framework OpenGL"
		"-framework CoreFoundation"
		"-framework Carbon"
		${NFD_LIBRARY_MAC}
		glfw
		${Freetype_LIBRARY}
	)
endif()

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
