#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"
#include "Cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Texture {

public:
	unsigned int textureID;
	int textureWidth;
	int textureHeight;
	int colourChannels;
	unsigned char* data;

	Texture() {

	}

	void CreateTexture() {

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); //ALL TEXTURE OPERATIONS WILL AFFECT THIS TEXTURE

		//TEXTURE PARAMETERS
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//TEXTURE FILTERING
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps

		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		data = stbi_load("C++OpenGL\container.jpg", &textureWidth, &textureHeight, &colourChannels, 0);
		if (data)
		{
			if (colourChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else if (colourChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "ERROR -> UKNOWN NUMBER OF COLOUR CHANNELS !" << std::endl;
			}

		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	}

	void BindTexture() {
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

};