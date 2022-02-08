#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"

using namespace std;




//INITIALOZE METHODS
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void CloseWindowOnInput(GLFWwindow* window);

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


	

	//***********************
	//SHADER COMPILATION
	//**********************

	Shader shader("vertexShader.vert","fragmentShader.frag");



	//*******************
	// VBO AND VAO 
	// ******************
	float vertices[] = {
		// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//SEND COLOURS TO SHADER AT LOCATION 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//SEND TEXTURE DATA TO SHADER AT LOCATION 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
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
		glClear(GL_COLOR_BUFFER_BIT);	//CLEAR FRANE 

		//TEXTURE RENDER STUFF
		glBindTexture(GL_TEXTURE_2D, textureID);

		shader.UseShader();
		glBindVertexArray(VAO);	//BIND VAO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
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

int main() {

	int windowWidth = 800;
	int windowHeight = 600;


	INITIALIZE(windowWidth, windowHeight);
	
	return 0;
}