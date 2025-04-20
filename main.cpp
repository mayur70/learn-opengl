#include <iostream>
#include <array>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "ogl.hpp"
#include "platform.hpp"

constexpr int width = 800;
constexpr int height = 600;

struct Vector2 {
	float x, y;
};
struct Vector4 {
	float x, y, z, w;
};
struct Vector3 {
	float x, y, z;
};

struct VertexElementLayout {
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* offset;
};
struct Vertex {
	Vector3 position;
	Vector4 color;
	Vector2 texture_coord;

	static std::array<VertexElementLayout, 2> get_layout() {
		std::array<VertexElementLayout, 2> layout{};
		layout[0].size = 3;
		layout[0].type = GL_FLOAT;
		layout[0].normalized = GL_FALSE;
		layout[0].stride = sizeof(Vertex);
		layout[0].offset = (const void*)offsetof(Vertex, position);
		layout[1].size = 4;
		layout[1].type = GL_FLOAT;
		layout[1].normalized = GL_FALSE;
		layout[1].stride = sizeof(Vertex);
		layout[1].offset = (const void*)offsetof(Vertex, color);
		layout[2].size = 2;
		layout[2].type = GL_FLOAT;
		layout[2].normalized = GL_FALSE;
		layout[2].stride = sizeof(Vertex);
		layout[2].offset = (const void*)offsetof(Vertex, texture_coord);
		return layout;
	}
};

struct Texture2D {
	gl::Texture handle;
	int w, h, nr_channels;

	Texture2D(std::string_view filename)
	: handle{}, w{0}, h{0}, nr_channels{0}
	{
		auto *data = stbi_load(filename.data(), &w, &h, &nr_channels, STBI_default);
		if(!data) {
			std::cerr << "failed to load texture from file " << filename << "\n";
			std::abort();
		}
		handle.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
};

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
	{
		auto* window_handle = glfwCreateWindow(width, height, "learn-opengl", nullptr, nullptr);
		glfw3::Window window{glfw3::make_window(window_handle)};
		glfwMakeContextCurrent(window.get());

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cerr << "failed to load opengl\n";
			std::abort();
		}

		glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

		glViewport(0, 0, width, height);
		{
			auto vsrc = fs::read_all("res/basic.vert");
			auto fsrc = fs::read_all("res/basic.frag");
			auto vs = gl::Shader::make(GL_VERTEX_SHADER, vsrc);
			auto fs = gl::Shader::make(GL_FRAGMENT_SHADER, fsrc);
			auto shader_program = gl::ShaderProgram::make(vs, fs);

			std::array<Vertex, 4> vertices{
				Vertex{{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
				Vertex{{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
				Vertex{{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
				Vertex{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			};

			gl::check_errors(__FILE__, __LINE__);
			gl::VertexArray vao{};
			vao.bind();
			gl::Buffer vbo{gl::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.size() * sizeof(Vertex), vertices.data())};
			unsigned int indices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};
			gl::Buffer ebo{gl::Buffer{GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices}};

			//Texture2D texture{"res/wall.jpg"};

			gl::check_errors(__FILE__, __LINE__);
			const auto layout = Vertex::get_layout();
			for(GLuint index = 0; index < layout.size(); index++) {
				auto* el = &layout[index];
				glVertexAttribPointer(index, el->size, el->type, el->normalized, el->stride, el->offset);
				glEnableVertexAttribArray(index);
			}
			gl::check_errors(__FILE__, __LINE__);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			while(!glfwWindowShouldClose(window.get())) {

				glfwPollEvents();
				process_input(window.get());

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				shader_program.bind();
				std::cout << "shaderprog bind " << shader_program.id << "\n";
				gl::check_errors(__FILE__, __LINE__);
				vao.bind();
				gl::check_errors(__FILE__, __LINE__);
				ebo.bind();
				gl::check_errors(__FILE__, __LINE__);
			//	texture.handle.bind();
				//glDrawArrays(GL_TRIANGLES, 0, 3);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				gl::check_errors(__FILE__, __LINE__);
				glfwSwapBuffers(window.get());
			}
		}
	}
	glfwTerminate();
	return 0;
}
