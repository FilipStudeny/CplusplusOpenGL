#version 330 core   //VERSION OF OPENGL 3.3 CORE PROFILE

layout(location = 0) in vec3 vertexPosition; //INPUT DATA -> VERTICES


uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vertexPosition, 1.0);

}
