#pragma once

#include <memory>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glfw3 {
using Window = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;
Window make_window(GLFWwindow* handle);
}

namespace fs {
std::string read_all(std::string_view filepath);
}

