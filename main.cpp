#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

constexpr int width = 800;
constexpr int height = 600;


void framebuffer_size_callback([[maybe_unused]] GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
}

void process_input([[maybe_unused]] GLFWwindow* win) {
	if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, GLFW_TRUE);
}

int main([[maybe_unused]] int argc,[[maybe_unused]] char* argv[]) {
	if(glfwInit() != GLFW_TRUE) {
		std::cerr << "failed to init GLFW\n";
		std::abort();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	auto* window = glfwCreateWindow(width, height, "learn-opengl", nullptr, nullptr);
	if(!window) {
		std::cerr << "failed to create window\n";
		std::abort();
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cerr << "failed to load opengl\n";
		std::abort();
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, width, height);

	while(!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
