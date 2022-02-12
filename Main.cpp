#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "VaoVboEbo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//INITIALOZE METHODS
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void Transforms(Shader shader, int width, int height);
void worldMatrixCalc(Shader shader, int i, glm::vec3 position);
void MouseRotationCallback(GLFWwindow* window, double xposIn, double yposIn);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int width = 800;
int height = 600;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


float lastMousePositionX = width / 2.0f;
float lastMousePositionY = height / 2.0f;
bool mouseEnteredWindow = true;

Camera camera(width, height);

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};



int main() {


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
	//CALLBACK SETUPS
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseRotationCallback);
	glfwSetScrollCallback(window, MouseScrollCallback); 

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//CHECK FOR GLAD INTIALIZE
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INITIALIZE GLAD ! " << std::endl;
		return -1;
	}

	//SET WINDOW PROPERTIES
	glViewport(0, 0, width, height); //SET WINDOW WIDTH AND HEIGHT



	glEnable(GL_DEPTH_TEST);

	//***********************
	//SHADER COMPILATION
	//**********************
	Shader shader("vertexShader.vert", "fragmentShader.frag");


	//*******************
	// VBO AND VAO 
	// ******************
	VAO vao;
	vao.Bind();

	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));

	vao.LinkVBO(vbo, 0,3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkVBO(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();


	
	//*******************
	// OBJECTS POSITIONS 
	// ******************
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





	//**********************
	//	TEXTURE 
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
	if (data){
		if (colourChannels == 3) {
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
	stbi_image_free(data);	//FREE MEMORY



	//LOOP WINDOW -> ONE RENDER LOOP ---> FRAME
	while (!glfwWindowShouldClose(window))
	{
		//DELTA TIME CALC
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//INPUT
		camera.KeyboardInput(window, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

	


		//RENDER STUFF HERE !
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//SET BACKGROUND COLOR 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//CLEAR FRANE 

		//TEXTURE RENDER STUFF
		glBindTexture(GL_TEXTURE_2D, textureID);

		//SHADER PROCESS
		shader.UseShader();
		shader.setVec3("objectColour", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);

		Transforms(shader, width, height);


		vao.Bind();

		for (unsigned int i = 0; i < 10; i++)
		{
			worldMatrixCalc(shader, i, cubePositions[i]);
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		}


		//CHECK FOR EVENTS CALLED BY USER OR APP
		glfwSwapBuffers(window);  //SWAP COLOUR BUFFER
		glfwPollEvents(); //CHECK FOR EVENT TRIGGERS
	}


	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


//WINDOW CALLBACK FOR WINDOW RESIZE - FIXED VIEWPORT ON RESIZE
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Transforms(Shader shader, int width, int height) {

	float aspectRatio = (float)width / (float)height;
	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(camera.GetFOV()), aspectRatio,camera.GetNearPlane(), camera.GetFarPlane());
	shader.setMat4("projectionMatrix", projectionMatrix);

	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("viewMatrix", viewMatrix);

	
}

void worldMatrixCalc(Shader shader, int i, glm::vec3 position) {
	glm::mat4 worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, position);
	float angle = 20.0f * i;
	worldMatrix = glm::rotate(worldMatrix, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	shader.setMat4("worldMatrix", worldMatrix);
}

void MouseRotationCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (mouseEnteredWindow)
	{
		lastMousePositionX = xpos;
		lastMousePositionY = ypos;
		mouseEnteredWindow = false;
	}

	float xoffset = xpos - lastMousePositionX;
	float yoffset = lastMousePositionY - ypos; // reversed since y-coordinates go from bottom to top

	lastMousePositionX = xpos;
	lastMousePositionY = ypos;
	camera.MousePositionUpdate(xoffset, yoffset);

}

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.MouseScrollUpdate((float)yoffset);
}
