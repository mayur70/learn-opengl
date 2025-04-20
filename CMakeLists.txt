cmake_minimum_required(VERSION 3.20)

project(learn-opengl)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory("external/glfw")

add_executable(learn-opengl
	main.cpp
)
target_link_libraries(learn-opengl
	PRIVATE
	glfw
)
