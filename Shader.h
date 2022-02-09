#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
	unsigned int shaderID;

	Shader(const char* vertexShaderSRC, const char* fragmentShaderSRC) {

		//RETRIEVE SHADER SRC FROM FILES
		std::string vertexShader;
		std::string fragmentShader;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		//ENSURE IFSTREAM CAN THROW EXCEPTION
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//OPEN FILES AND READ ITS CONTENT
			vertexShaderFile.open(vertexShaderSRC);
			fragmentShaderFile.open(fragmentShaderSRC);

			std::stringstream vertexShaderStream;
			std::stringstream fragmentShaderStream;

			//READ FILE CONTENTS INTO STREAMS
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			//CLOSE FILES
			vertexShaderFile.close();
			fragmentShaderFile.close();

			//CONVERT DATA FROM STREAMS TO STRING
			vertexShader = vertexShaderStream.str();
			fragmentShader = fragmentShaderStream.str();

		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR -> COULDN'T READ FILE DATA " << e.what() << std::endl;
		}

		const char* vertexShaderData = vertexShader.c_str();
		const char* fragmentShaderData = fragmentShader.c_str();

		//COMPILE SHADERS
		unsigned int vertexID;
		unsigned int fragmentID;


		//VERTEX SHADER
		vertexID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexID, 1, &vertexShaderData, NULL);
		glCompileShader(vertexID);
		CheckCompileErrors(vertexID, "VERTEX");

		//FRAGMENT SHADER
		fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentID, 1, &fragmentShaderData, NULL);
		glCompileShader(fragmentID);
		CheckCompileErrors(fragmentID, "FRAGMENT");

		//LINK SHADERS
		shaderID = glCreateProgram();
		glAttachShader(shaderID, vertexID);
		glAttachShader(shaderID, fragmentID);
		glLinkProgram(shaderID);
		CheckCompileErrors(shaderID, "PROGRAM");

		//DELETE SHADERS AFTER COMPILE AND LINK
		glDeleteShader(vertexID);
		glCompileShader(fragmentID);

	}

	void UseShader() {
		glUseProgram(shaderID);
	}
	void setBool(const std::string& name, bool value) const {
		unsigned int location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1i(location, (int)value);
	}
	void setInt(const std::string& name, int value) const {
		unsigned int location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1i(location, (int)value);
	}
	void setFloat(const std::string& name, float value) const {
		unsigned int location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1f(location, value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	}

	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	unsigned int GetShaderID() { return shaderID; }

private:
	void CheckCompileErrors(unsigned int shaderID, std::string shaderType) {
		int success;
		char errorLog[1024];

		if (shaderType != "PROGRAM") {
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

			if (!success) {
				glGetShaderInfoLog(shaderID, 1024, NULL, errorLog);
				std::cout << "ERROR ->" << shaderType << " SHADER COMPILATION FAILED !" << "\n\t\t" << errorLog << std::endl;
			}
		}
		else {
			glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

			if (!success) {
				glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
				std::cout << "ERROR ->" << shaderType << " SHADER LINKING FAILED !" << "\n\t\t" << errorLog << std::endl;
			}
		}
	}

};

#endif

