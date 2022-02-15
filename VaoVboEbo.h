#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 UVmap;
};


class VBO {

private:
	GLuint ID;

public:
	VBO(std::vector<Vertex>& vertices);

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
	EBO(std::vector<GLuint>& triangles);

	void Bind();
	void Unbind();
	void Delete();
};

