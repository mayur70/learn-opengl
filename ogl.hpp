#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

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
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(const Shader&&) = delete;

	static Shader make(GLenum type, const std::string& source) ;
};

struct ShaderProgram {
	GLuint id;
	std::unordered_map<std::string, GLint> uniforms_cache;
	std::unordered_map<std::string, GLint> attributes_cache;
	ShaderProgram() ;
	~ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&&) = delete;
	ShaderProgram& operator=(const ShaderProgram&&) = delete;

	void bind() ;

	void unbind() ;
	GLint get_uniform_location(const std::string& name);
	GLint get_attribute_location(const std::string& name);


	static ShaderProgram make(Shader& vs, Shader& fs) ;
};

struct Texture {
	GLuint id;
	int w, h, nr_channels;

	Texture();
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(const Texture&&) = delete;

	void bind(GLuint slot = 0);
	void unbind();
};
}

