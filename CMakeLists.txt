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
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   OFF)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
  message("MSVC Version: ${MSVC_VERSION} | Toolset: ${MSVC_TOOLSET_VERSION}")
endif()


file(GLOB TOP_SRC_FILES "*.cpp" "*.h")

file(
	GLOB_RECURSE SRC_FILES
	"Common/*.h" "Common/*.cpp"
	"Deps/ImGui/*.h" "Deps/ImGui/*.cpp"
	"Model/*.h" "Model/*.cpp"
	"Widgets/*.h" "Widgets/*.cpp"
)

find_package(Freetype REQUIRED)
find_package(GLEW     REQUIRED)
find_package(glfw3    REQUIRED)
find_package(OpenGL   REQUIRED)

set(SPDLOG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/spdlog/include")
set(IMGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/ImGui")
set(NFD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/src/include")
set(NFD_LIBRARY_MAC "${PROJECT_SOURCE_DIR}/Deps/NativeFileDialog114/build/lib/Release/x64/libnfd.a")
set(JSON_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/nlohmann/")
set(GLM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Deps/glm/")

message("json @ ${JSON_INCLUDE_DIR}")
add_executable (
	${PROJECT_NAME}
	${TOP_SRC_FILES}
	${SRC_FILES}
	${SPDLOG_SRC_FILSE}
)

if (WIN32)
	include_directories(
		${Freetype_INCLUDE_DIRS}
		${GLM_INCLUDE_DIR}
		${glfw3_INCLUDE_DIR}
		${OpenGL_INCLUDE_DIR}
		# In /Deps
		${JSON_INCLUDE_DIR}
		${IMGUI_INCLUDE_DIR}
		${NFD_INCLUDE_DIR}
		${SPDLOG_INCLUDE_DIR}
	)
    target_link_libraries(
		${PROJECT_NAME}
		-lserialport
		${GLEW_LIBRARY}
		${GLFW3_LIBRARY}
		${Freetype_LIBRARY}
	)

# Linux
elseif (UNIX AND NOT APPLE)

	include_directories(
		${Freetype_INCLUDE_DIRS}
		${glfw3_INCLUDE_DIR}
		${OpenGL_INCLUDE_DIR}
		# In /Deps
		${GLM_INCLUDE_DIR}
	    ${JSON_INCLUDE_DIR}
		${IMGUI_INCLUDE_DIR}
    	${NFD_INCLUDE_DIR}
		${SPDLOG_INCLUDE_DIR}
	)
    target_link_libraries(
		${PROJECT_NAME}
		#libGrbl
		-lpthread
		-ldl
		-lserialport
		glfw
		${GLEW_LIBRARY}
		${Freetype_LIBRARY}
		${OPENGL_LIBRARIES}
	)

elseif (APPLE)
	include_directories(
		${Freetype_INCLUDE_DIRS}
		${glfw3_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		# In /Deps
		${GLM_INCLUDE_DIR}
		${OpenGL_INCLUDE_DIR}
		${IMGUI_INCLUDE_DIR}
    	${NFD_INCLUDE_DIR}
		${JSON_INCLUDE_DIR}
		${SPDLOG_INCLUDE_DIR}
	)
    target_link_libraries(
		${PROJECT_NAME}
		#libGrbl
		-lpthread
		-ldl
		"-framework IOKit"
		"-framework AppKit"
		"-framework OpenGL"
		"-framework CoreFoundation"
		"-framework Carbon"
		/usr/local/lib/libserialport.a
		${NFD_LIBRARY_MAC}
		glfw
		${GLEW_LIBRARY}
		${Freetype_LIBRARY}
	)
endif()

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
