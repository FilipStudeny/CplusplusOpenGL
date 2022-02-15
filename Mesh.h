#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "Camera.h"
#include "VaoVboEbo.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh {

private:
	std::vector <Vertex> vertices;
	std::vector <GLuint> triangles;
	std::vector <Texture> textures;

public:
	VAO meshVAO;
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& triangles, std::vector <Texture>& textures);
	void DrawMesh(Shader& shader, Camera& camera, glm::vec3 position, float scale, float angle);
};