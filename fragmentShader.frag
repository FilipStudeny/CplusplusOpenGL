//CALCULATING COLOUR OUTPUT FOR PIXELS

#version 330 core

//OUTPUT 
out vec4 Color; 

//INPUT 
in vec4 fragColour;

void main(){

	Color = fragColour;
}
