#version 330 core
out vec4 FragColor;

uniform vec4 lightColour;

void main()
{
    FragColor = lightColour; // set alle 4 vector values to 1.0
}