#version 330 core	//CALCULATING COLOUR OUTPUT FOR PIXELS


//OUTPUT 
out vec4 Color; 

//INPUT 
in vec4 fragColour;
in vec2 fragTextureCoords;

//UNIFORMS
uniform sampler2D textureSampler;

void main(){

	Color = texture(textureSampler, fragTextureCoords);
}
