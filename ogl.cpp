#include "ogl.hpp"

#include <iostream>

bool gl::check_errors(const char* filename, long line) {
	bool is_err = false;
	GLenum err = GL_NO_ERROR;
	while((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error "<< err << " at " << filename << ":"<< line<<"\n";
		is_err = true;
	}
	return is_err;
}

gl::Buffer::Buffer(GLenum target, GLenum usage, GLsizeiptr size, GLvoid* data)
:id{0},target{target}
{
	glGenBuffers(1, &id);
	bind();
	glBufferData(target, size, data, usage);
	std::cout << "Buffer created "<< id <<"\n";
}

gl::Buffer::~Buffer() {
	glDeleteBuffers(1, &id);
	std::cout << "Buffer deleted "<< id <<"\n";
}

void gl::Buffer::bind() {
	glBindBuffer(target, id);
}
void gl::Buffer::unbind() {
	glBindBuffer(target, 0);
}

gl::VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
	std::cout << "VertexArray created "<< id <<"\n";
}
gl::VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
	std::cout << "VertexArray deleted "<< id <<"\n";
}

void gl::VertexArray::bind() {
	glBindVertexArray(id);
}
void gl::VertexArray::unbind() {
	glBindVertexArray(0);
}

gl::Shader::Shader(GLenum type)
:id{glCreateShader(type)}
{
	std::cout << "Shader created "<< id <<"\n";
}

gl::Shader::~Shader()
{
	glDeleteShader(id);
	std::cout << "Shader deleted "<< id <<"\n";
}

gl::Shader gl::Shader::make(GLenum type, const std::string& source) {
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

gl::ShaderProgram::ShaderProgram() 
:id{glCreateProgram()}
{
	std::cout << "ShaderProgram created "<< id <<"\n";
}
gl::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
	std::cout << "ShaderProgram deleted "<< id <<"\n";
}

void gl::ShaderProgram::bind() {
	glUseProgram(id);
}

void gl::ShaderProgram::unbind() {
	glUseProgram(0);
}

gl::ShaderProgram gl::ShaderProgram::make(Shader& vs, Shader& fs) {
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
