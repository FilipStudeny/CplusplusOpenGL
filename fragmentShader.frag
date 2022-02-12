#version 330 core	//CALCULATING COLOUR OUTPUT FOR PIXELS


//OUTPUT 
out vec4 Color; 

//INPUT 
in vec4 fragColour;
in vec2 fragTextureCoords;

//UNIFORMS
uniform sampler2D textureSampler;
uniform vec3 objectColour;
uniform vec3 lightColour;

void main(){

	Color = texture(textureSampler, fragTextureCoords);
}
