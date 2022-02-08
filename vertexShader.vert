#version 330 core   //VERSION OF OPENGL 3.3 CORE PROFILE

layout(location = 0) in vec3 vertexPosition; //INPUT DATA -> VERTICES

void main(){

	gl_Position = vec4(vertexPosition, 1.0);

}
