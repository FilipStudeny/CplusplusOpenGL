#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Texture::Texture(const char* texturePath, const char* textureType, GLuint textureSlot) {
	this->textureType = textureType;
	this->textureSlot = textureSlot;

	int textureWidth;
	int textureHeight;
	int colourChannels;
	unsigned char* data;
	data = stbi_load(texturePath, &textureWidth, &textureHeight, &colourChannels, 0);

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + this->textureSlot);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	//TEXTURE PARAMETERS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//TEXTURE FILTERING
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		if (colourChannels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

		}else if (colourChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

		}else if (colourChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

		}else {
			std::cout << "ERROR -> UKNOWN NUMBER OF COLOUR CHANNELS !" << std::endl;
		}
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UseTexture(Shader shader, const char* uniform, GLuint unit) {
	GLuint textureUniform = glGetUniformLocation(shader.GetShaderID(), uniform);
	shader.UseShader();
	glUniform1i(textureUniform, unit);
}

void Texture::Bind() {

	glActiveTexture(GL_TEXTURE0 + this->textureSlot);
	glBindTexture(GL_TEXTURE_2D, this->textureID); //ALL TEXTURE OPERATIONS WILL AFFECT THIS TEXTURE

}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &this->textureID);
}

