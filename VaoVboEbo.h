#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VBO {

private:
	GLuint ID;

public:
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

};


class VAO {

private:
	GLuint ID;

public:
	VAO();

	void LinkVBO(VBO& vbo, GLuint location, GLuint sizeOfArrayedData, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();

};




class EBO {

private:
	GLuint ID;

public:
	EBO(GLuint* triangles, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

