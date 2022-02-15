#version 330 core   //VERSION OF OPENGL 3.3 CORE PROFILE

layout(location = 0) in vec3 vertexPosition; //INPUT DATA -> VERTICES
layout(location = 1) in vec3 vertexNormals;
layout(location = 2) in vec3 vertexColour;
layout(location = 3) in vec2 textureCoords;



//LIGHT CALC
out vec3 currePosition;
out vec3 fragObjectNormals;
out vec4 fragColour;
out vec2 fragTextureCoords;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){

	currePosition = vec3(worldMatrix * vec4(vertexPosition, 1.0f));

	gl_Position = projectionMatrix * viewMatrix * vec4(currePosition, 1.0);
	fragColour = vec4(vertexColour, 1.0);
	fragTextureCoords = textureCoords;
	fragObjectNormals = vertexNormals;
}
