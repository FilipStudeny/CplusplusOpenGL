#version 330 core   //VERSION OF OPENGL 3.3 CORE PROFILE

layout(location = 0) in vec3 vertexPosition; //INPUT DATA -> VERTICES
layout(location = 1) in vec3 vertexColour;
layout(location = 2) in vec2 textureCoords;

out vec4 fragColour;
out vec2 fragTextureCoords;

void main(){

	gl_Position = vec4(vertexPosition, 1.0);
	fragColour = vec4(vertexColour, 1.0);
	fragTextureCoords = textureCoords;
}
