cmake_minimum_required (VERSION 2.8.0)
project(CoconutCNC)

file(GLOB_RECURSE SRC_FILES "*.h" "*.cpp")

find_package(Freetype REQUIRED)
find_package(glm      REQUIRED)
find_package(GLEW     REQUIRED)
find_package(glfw3    REQUIRED)
find_package(OpenGL   REQUIRED)
find_package(spdlog   CONFIG REQUIRED)



add_executable (
	${PROJECT_NAME}
	${SRC_FILES}
)

if (WIN32)
	include_directories(
    ${Freetype_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${glfw3_INCLUDE_DIR}
    ${OpenGL_INCLUDE_DIR}
    ${spdlog_INCLUDE_DIR}
    "${PROJECT_SOURCE_DIR}/Deps/ImGui"
    "${PROJECT_SOURCE_DIR}/Deps/ImFileSystem"
)
    target_link_libraries(
		${PROJECT_NAME}
		#libGrbl
		${GLEW_LIBRARY}
		${GLFW3_LIBRARY}
		${Freetype_LIBRARY}
	)

# Linux
elseif (UNIX AND NOT APPLE)

	include_directories(
		${Freetype_INCLUDE_DIRS}
		${GLM_INCLUDE_DIR}
		${glfw3_INCLUDE_DIR}
		${OpenGL_INCLUDE_DIR}
		${spdlog_INCLUDE_DIR}
		"${PROJECT_SOURCE_DIR}/Deps/ImGui"
		"${PROJECT_SOURCE_DIR}/Deps/ImFileSystem"
	)
    target_link_libraries(
		${PROJECT_NAME}
		#libGrbl
		-lpthread
		-ldl
		${GLEW_LIBRARY}
		glfw
		${Freetype_LIBRARY}
		${OPENGL_LIBRARIES}
	)

elseif (APPLE)
	include_directories(
		${Freetype_INCLUDE_DIRS}
		${GLM_INCLUDE_DIR}
		${glfw3_INCLUDE_DIR}
		${OpenGL_INCLUDE_DIR}
		"${PROJECT_SOURCE_DIR}/Deps/ImGui"
		"${PROJECT_SOURCE_DIR}/Deps/ImFileSystem"
	)
    target_link_libraries(
		${PROJECT_NAME}
		#libGrbl
		-lpthread
		-ldl
		"-framework OpenGL"
		"-framework CoreFoundation"
		"-framework Carbon"
		${GLEW_LIBRARY}
		glfw
		${Freetype_LIBRARY}
		spdlog::spdlog
	)
endif()

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Client/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
