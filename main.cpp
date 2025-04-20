#include <iostream>

#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
	if(glfwInit() != GLFW_TRUE) {
		std::cerr << "failed to init GLFW\n";
		std::abort();
	}
	
	auto* window = glfwCreateWindow(800, 480, "learn-opengl", nullptr, nullptr);
	if(!window) {
		std::cerr << "failed to create window\n";
		std::abort();
	}

	glfwMakeContextCurrent(window);

	while(!glfwWindowShouldClose(window)) {

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
