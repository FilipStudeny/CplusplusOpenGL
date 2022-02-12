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
uniform vec3 cameraPosition;

void main(){

	vec3 normal = normalize(fragObjectNormals);
	vec3 lightDirection = normalize(lightPosition - currePosition);

	//DIFFUSE LIGHTING
	float diffuseLight = max(dot(normal, lightDirection), 0.0f);
	float ambientLight = 0.20f;
	float specularLightPower = 0.50f; //MAX INTENSITY OF LIGHT

	//SPECULAR LIGHT CALC
	vec3 cameraViewDirection = normalize(cameraPosition - currePosition);
	vec3 reflectionDirection = reflect(-lightDirection, fragObjectNormals);
	float specularLightAmount = pow(max(dot(cameraViewDirection, reflectionDirection), 0.0f), 8);
	float specularLight = specularLightPower * specularLightAmount;

	Color = texture(textureSampler, fragTextureCoords) * lightColour * (diffuseLight + ambientLight + specularLight);
}
