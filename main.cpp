#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

constexpr int width = 800;
constexpr int height = 600;

namespace glfw3 {
using Window = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;
Window make_window(GLFWwindow* handle) {
	return Window{handle, glfwDestroyWindow};
}
}

namespace gl {

bool check_errors(const char* filename, long line) {
	bool is_err = false;
	GLenum err = GL_NO_ERROR;
	while((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error "<< err << " at " << filename << ":"<< line<<"\n";
		is_err = true;
	}
	return is_err;
}

struct Buffer {
	GLuint id;
	GLenum target;
	Buffer(GLenum target, GLenum usage, GLsizeiptr size, GLvoid* data)
	:id{0},target{target}
	{
		glGenBuffers(1, &id);
		bind();
		glBufferData(target, size, data, usage);
		std::cout << "Buffer created "<< id <<"\n";
	}

	~Buffer() {
		glDeleteBuffers(1, &id);
		std::cout << "Buffer deleted "<< id <<"\n";
	}

	void bind() {
		glBindBuffer(target, id);
	}
	void unbind() {
		glBindBuffer(target, 0);
	}
};

struct VertexArray {
	GLuint id;
	VertexArray() {
		glGenVertexArrays(1, &id);
		std::cout << "VertexArray created "<< id <<"\n";
	}
	~VertexArray() {
		glDeleteVertexArrays(1, &id);
		std::cout << "VertexArray deleted "<< id <<"\n";
	}

	void bind() {
		glBindVertexArray(id);
	}
	void unbind() {
		glBindVertexArray(0);
	}
};

struct Shader {
	GLuint id;
	Shader(GLenum type)
	:id{glCreateShader(type)}
	{
		std::cout << "Shader created "<< id <<"\n";
	}

	~Shader()
	{
		glDeleteShader(id);
		std::cout << "Shader deleted "<< id <<"\n";
	}

	static Shader make(GLenum type, const std::string& source) {
		Shader s{type};
		const char* src = source.c_str();
		glShaderSource(s.id, 1, &src, nullptr);
		glCompileShader(s.id);
		int success;
		char info_log[512];
		glGetShaderiv(s.id, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(s.id, 512, nullptr, info_log);
			std::cerr << "failed to compile shader (" << type << ") : " << info_log << "\n";
			std::abort();
		}
		return s;
	}
};

struct ShaderProgram {
	GLuint id;
	ShaderProgram() 
	:id{glCreateProgram()}
	{
		std::cout << "ShaderProgram created "<< id <<"\n";
	}
	~ShaderProgram() {
		glDeleteProgram(id);
		std::cout << "ShaderProgram deleted "<< id <<"\n";
	}

	void bind() {
		glUseProgram(id);
	}

	void unbind() {
		glUseProgram(0);
	}

	static ShaderProgram make(Shader& vs, Shader& fs) {
		ShaderProgram prog{};
		glAttachShader(prog.id, vs.id);
		glAttachShader(prog.id, fs.id);
		glLinkProgram(prog.id);
		int success;
		char info_log[512];
		glGetProgramiv(prog.id, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(prog.id, 512, nullptr, info_log);
			std::cerr << "failed to link shaders " << info_log << "\n";
			std::abort();
		}
		return prog;
	}
};
}

namespace fs {
std::string read_all(std::string_view filepath) {
	std::ifstream file;
	file.open(filepath.data());
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}
}

#define GLC(x) \
do { \
	x; \
	gl::check_errors(__FILE__, __LINE__); \
} while(0);

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

		auto vsrc = fs::read_all("res/basic.vert");
		auto fsrc = fs::read_all("res/basic.frag");
		auto vs = gl::Shader::make(GL_VERTEX_SHADER, vsrc);
		auto fs = gl::Shader::make(GL_FRAGMENT_SHADER, fsrc);
		auto shader_program = gl::ShaderProgram::make(vs, fs);
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};  
		gl::check_errors(__FILE__, __LINE__);
		gl::VertexArray vao{};
		vao.bind();
		gl::Buffer vbo{gl::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices)};

		gl::check_errors(__FILE__, __LINE__);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		gl::check_errors(__FILE__, __LINE__);

		while(!glfwWindowShouldClose(window.get())) {

			glfwPollEvents();
			process_input(window.get());

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shader_program.bind();
			vao.bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);


			glfwSwapBuffers(window.get());
		}
	}
	glfwTerminate();
	return 0;
}
