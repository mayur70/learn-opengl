#include "platform.hpp"

#include <fstream>
#include <sstream>

glfw3::Window glfw3::make_window(GLFWwindow* handle) {
	return Window{handle, glfwDestroyWindow};
}

std::string fs::read_all(std::string_view filepath) {
	std::ifstream file;
	file.open(filepath.data());
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}


