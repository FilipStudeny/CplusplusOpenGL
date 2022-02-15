#include "VaoVboEbo.h"

//***********
//	VAO
//***********
VAO::VAO() {
	glGenVertexArrays(1, &this->ID);
}

void VAO::LinkVBO(VBO& vbo, GLuint location, GLuint sizeOfArrayedData, GLenum type, GLsizeiptr stride, void* offset) {

	vbo.Bind();
	glVertexAttribPointer(location, sizeOfArrayedData, type, GL_FALSE, stride, offset);	//SEND DATA TO SHADER AT LOCATION <location>
	glEnableVertexAttribArray(location);
	vbo.Unbind();
}

void VAO::Bind() { glBindVertexArray(this->ID); }
void VAO::Unbind() { glBindVertexArray(0); }
void VAO::Delete() { glDeleteVertexArrays(1, &this->ID); }

//***********
//	VBO
//***********
VBO::VBO(std::vector<Vertex>& vertices){
	glGenBuffers(1, &this->ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	/*
	COPY VERTEX DATA INTO BUFFER
	--> GL_ARRAY_BUFFER --> WHERE TO SEND DATA
	--> sizeof(vertices) --> SIZE OF DATA IN BYTES TO PASS TO THE BUFFER
	--> VERTICES --> DATA TO BE SEND TO THE BUFFER
	--> GL_STATIC_DRAW --> HOW TO PROCESS DATA
	*/
}

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, this->ID); }
void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VBO::Delete() { glDeleteBuffers(1, &this->ID); }

//***********
//	EBO
//***********
EBO::EBO(std::vector<GLuint>& triangles) {
	glGenBuffers(1, &this->ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(GLuint), triangles.data(), GL_STATIC_DRAW);
}


void EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID); }
void EBO::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void EBO::Delete() { glDeleteBuffers(1, &this->ID); }
