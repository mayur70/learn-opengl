#pragma once
#include <string>

#include <glad/glad.h>

#define GLC(x) \
do { \
	x; \
	gl::check_errors(__FILE__, __LINE__); \
} while(0);

namespace gl {

bool check_errors(const char* filename, long line); 

struct Buffer {
	GLuint id;
	GLenum target;
	Buffer(GLenum target, GLenum usage, GLsizeiptr size, GLvoid* data);

	~Buffer() ;

	void bind();
	void unbind();
};

struct VertexArray {
	GLuint id;
	VertexArray() ;
	~VertexArray() ;

	void bind() ;
	void unbind() ;
};

struct Shader {
	GLuint id;
	Shader(GLenum type);

	~Shader();

	static Shader make(GLenum type, const std::string& source) ;
};

struct ShaderProgram {
	GLuint id;
	ShaderProgram() ;
	~ShaderProgram();

	void bind() ;

	void unbind() ;

	static ShaderProgram make(Shader& vs, Shader& fs) ;
};
}

