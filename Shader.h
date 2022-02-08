#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


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

