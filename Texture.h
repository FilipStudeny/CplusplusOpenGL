#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Texture {

private:
	GLuint textureID;
	GLuint textureSlot;

public:
	const char* textureType;

	Texture(const char* texturePath, const char* textureType, GLuint textureSlot);

	void UseTexture(Shader shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

};