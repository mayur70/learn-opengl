cmake_minimum_required(VERSION 3.20)

project(learn-opengl LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory("external/glfw" EXCLUDE_FROM_ALL)
add_subdirectory("external/glad" EXCLUDE_FROM_ALL)

add_executable(learn-opengl
	main.cpp
)
target_link_libraries(learn-opengl
	PRIVATE
	glfw
	Glad
)

set_property(TARGET learn-opengl
	PROPERTY CMAKE_CXX_STANDARD 20
)
set_property(TARGET learn-opengl
	PROPERTY CMAKE_CXX_STANDARD_REQUIRED TRUE
)

if(MSVC)
target_compile_options(learn-opengl 
	PRIVATE /W4 /WX
)
else()
target_compile_options(learn-opengl 
	PRIVATE -Wall -Wextra -Werror -Wpedantic
)
endif()
