#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "Camera.h"
#include "VaoVboEbo.h"
#include "Texture.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& triangles, std::vector <Texture>& textures) {
	this->vertices = vertices;
	this->triangles = triangles;
	this->textures = textures;

	this->meshVAO.Bind();

	VBO meshVBO(this->vertices);
	EBO meshEBO(this->triangles);

	this->meshVAO.LinkVBO(meshVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	this->meshVAO.LinkVBO(meshVBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	this->meshVAO.LinkVBO(meshVBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	this->meshVAO.LinkVBO(meshVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	this->meshVAO.Unbind();
	meshVBO.Unbind();
	meshEBO.Unbind();
}

void Mesh::DrawMesh(Shader& shader, Camera& camera, glm::vec3 position, float scale, float angle) {
	shader.UseShader();
	this->meshVAO.Bind();

	//TEXTURE ASSIGNMENT
	unsigned int diffuseTexture = 0;
	unsigned int specularTexture = 0;
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		std::string num;
		std::string textureType = this->textures[i].textureType;
		if (textureType == "diffuse") {
			num = std::to_string(diffuseTexture++);
		}
		else if (textureType == "specular") {
			num = std::to_string(specularTexture++);
		}
		this->textures[i].UseTexture(shader, (textureType + num).c_str(), i);
		this->textures[i].Bind();
	}

	
	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(camera.GetFOV()), camera.GetAspectRatio(), camera.GetNearPlane(), camera.GetFarPlane());
	shader.setMat4("projectionMatrix", projectionMatrix);

	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("viewMatrix", viewMatrix);

	glm::mat4 worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, position);
	worldMatrix = glm::scale(worldMatrix, glm::vec3(scale, scale, scale));
	worldMatrix = glm::rotate(worldMatrix, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	shader.setMat4("worldMatrix", worldMatrix);

	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);



}