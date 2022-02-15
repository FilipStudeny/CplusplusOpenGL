#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "Mesh.h"


//INITIALOZE METHODS
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
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
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}

};

// Indices for vertices order
GLuint triangles[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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
	//	TEXTURES
	//**********************
	Texture textures[]
	{

		Texture("planks.png","diffuse",0),
		Texture("planksSpec.png", "specular", 1)
	};

	//***********************
	//	OBJECT
	//**********************
	Shader shader("vertexShader.vert", "fragmentShader.frag");
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(triangles, triangles + sizeof(triangles) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);


	//*******************
	// LIGHT CUBE
	// ******************
	glm::vec3 lightPosition = glm::vec3(0.0f, 1.0f, -1.0f);
	glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Shader lightShader("lightShader.vert", "lightShader.frag");

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);


	//*******************
	// OBJECTS POSITIONS 
	// ******************
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  -2.0f),
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

			
		lightShader.UseShader();
		lightShader.setVec4("lightColour", lightColour);
		light.DrawMesh(lightShader, camera, lightPosition, 1, 0);


		shader.UseShader();
		shader.setVec4("lightColour", lightColour);
		shader.setVec3("lightPosition", lightPosition);
		shader.setVec3("cameraPosition", camera.GetCameraPosition());
		floor.DrawMesh(shader, camera, cubePositions[0], 2, 0);

	
		//CHECK FOR EVENTS CALLED BY USER OR APP
		glfwSwapBuffers(window);  //SWAP COLOUR BUFFER
		glfwPollEvents(); //CHECK FOR EVENT TRIGGERS
	}

	
	shader.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


//WINDOW CALLBACK FOR WINDOW RESIZE - FIXED VIEWPORT ON RESIZE
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
