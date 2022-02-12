#version 330 core	//CALCULATING COLOUR OUTPUT FOR PIXELS


//OUTPUT 
out vec4 Color; 

//INPUT 
in vec4 fragColour;
in vec2 fragTextureCoords;
in vec3 currePosition;
in vec3 fragObjectNormals;

//UNIFORMS
uniform sampler2D textureSampler;


//LIGHT CALC
uniform vec4 lightColour;
uniform vec3 lightPosition;

void main(){

	vec3 normal = normalize(fragObjectNormals);
	vec3 lightDirection = normalize(lightPosition - currePosition);

	//DIFFUSE LIGHTING
	float diffuseLight = max(dot(normal, lightDirection), 0.0f);

	Color = texture(textureSampler, fragTextureCoords) * lightColour * diffuseLight;
}
