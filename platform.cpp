#include "platform.hpp"
#include "GLFW/glfw3.h"

#include <fstream>
#include <sstream>
#include <iostream>

static void destroy_win(GLFWwindow* win) {
	glfwDestroyWindow(win);
	std::cout << "window destroyed\n";
}

glfw3::Window glfw3::make_window(GLFWwindow* handle) {
	return Window{handle, destroy_win};
}

std::string fs::read_all(std::string_view filepath) {
	std::ifstream file;
	file.open(filepath.data());
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}


