#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;




//INITIALOZE METHODS
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void CloseWindowOnInput(GLFWwindow* window);
void Transforms(Shader shader, int width, int height);
void worldMatrixCalc(Shader shader, int i, glm::vec3 position);

int INITIALIZE(int width, int height) {
	glfwInit(); //INITIALIZE GLFW


	//SETUP OPENGL VERSIONS FOR GLFW -> OPENGL3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //SET OPENGL PROFILE TO CORE

	//CREATE WINDOW
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);

	//TERMINATE GLFW IF WINDOW IS NOT CREATED
	if (window == NULL) {
		std::cout << "FAILED TO CREATE WINDOW !" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //MAKE WINDOW MAIN CONTEXT

	//CHECK FOR GLAD INTIALIZE
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INITIALIZE GLAD ! " << std::endl;
		return -1;
	}

	//SET WINDOW PROPERTIES
	glViewport(0, 0, width, height); //SET WINDOW WIDTH AND HEIGHT

	

	//CALLBACK SETUPS
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	glEnable(GL_DEPTH_TEST);

	

	//***********************
	//SHADER COMPILATION
	//**********************

	Shader shader("vertexShader.vert","fragmentShader.frag");



	//*******************
	// VBO AND VAO 
	// ******************
	float vertices[] = {
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//VAO CREATION
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//VBO CREATION
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //BIND BUFFERS TO ARRAY

	/*
	COPY VERTEX DATA INTO BUFFER
	--> GL_ARRAY_BUFFER --> WHERE TO SEND DATA
	--> sizeof(vertices) --> SIZE OF DATA IN BYTES TO PASS TO THE BUFFER
	--> VERTICES --> DATA TO BE SEND TO THE BUFFER
	--> GL_STATIC_DRAW --> HOW TO PROCESS DATA
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//EBO - ELEMENT BUFFER OBJECTS
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SEND VERTICES TO SHADER AT LOCATION 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//SEND COLOURS TO SHADER AT LOCATION 1
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//SEND TEXTURE DATA TO SHADER AT LOCATION 2

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//**********************
	//TEXTURE 
	//**********************
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID); //ALL TEXTURE OPERATIONS WILL AFFECT THIS TEXTURE

	//TEXTURE PARAMETERS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//TEXTURE FILTERING
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int textureWidth; 
	int textureHeight;
	int colourChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("container.jpg", &textureWidth, &textureHeight, &colourChannels, 0);
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



	//LOOP WINDOW -> ONE RENDER LOOP ---> FRAME
	while (!glfwWindowShouldClose(window))
	{

		//INPUT PROCESSS
		CloseWindowOnInput(window);

		//RENDER STUFF HERE !
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//SET BACKGROUND COLOR 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//CLEAR FRANE 

		//TEXTURE RENDER STUFF
		glBindTexture(GL_TEXTURE_2D, textureID);

		shader.UseShader();

		Transforms(shader, width, height);

		
		glBindVertexArray(VAO);	//BIND VAO

		for (unsigned int i = 0; i < 10; i++)
		{
			worldMatrixCalc(shader, i, cubePositions[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		//CHECK FOR EVENTS CALLED BY USER OR APP
		glfwSwapBuffers(window);  //SWAP COLOUR BUFFER
		glfwPollEvents(); //CHECK FOR EVENT TRIGGERS
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return -1;
}

//WINDOW CALLBACK FOR WINDOW RESIZE - FIXED VIEWPORT ON RESIZE
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void CloseWindowOnInput(GLFWwindow* window) {
	//GET ESCAPE INPUT
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Transforms(Shader shader,int width, int height) {

	//OBJECT ROTATION IN 2D
	/*
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5, 0.5, 0.5));
	vec = transformMatrix * vec;
	shader.setMat4("transformMatrix", transformMatrix);
	*/

	//3D SPACE
	

	float aspectRation = (float)width / (float)height;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
	shader.setMat4("viewMatrix", viewMatrix);

	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRation, nearPlane, farPlane);
	shader.setMat4("projectionMatrix", projectionMatrix);
}

void worldMatrixCalc(Shader shader,int i, glm::vec3 position) {
	glm::mat4 worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, position);
	float angle = 20.0f * i;
	worldMatrix = glm::rotate(worldMatrix, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	shader.setMat4("worldMatrix", worldMatrix);
}

int main() {

	int windowWidth = 800;
	int windowHeight = 600;


	INITIALIZE(windowWidth, windowHeight);
	
	return 0;
}