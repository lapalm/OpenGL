
#pragma once
#ifdef SHADERUTILITY_H
#define SHADERUTILITY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>; //Include glew to get all the required OpenGL headers.

class ShaderUtil 
{
public:
	// The program ID
	GLuint Program;

	/*We're using C++ filestreams to read the content from the file into several string objects:*/

	//Constructor reads and builds the shader
	ShaderUtil(const GLchar* vertexPath, const GLchar* fragmentPath) {
		//1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifsteam fShaderFile;

		// ensures ifstream objects can throw excceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {
			//Open Files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			//Convert stream into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failiure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		/*Next we need to compile and link the shaders. Note that we're also reviewing 
		if compilation/linking failed and if so, print the compile-time errors which is 
		extremely useful when debugging (you are going to need those error logs eventually):
		*/
		// 2. Compile shaders
		GLuint vertex, fragment;
		Glint success;
		GLchar infoLog[512];

		//Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//print compile errors if any
		glGetShader(vertex, GL_COMPULE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			td::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		//Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShadowSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		//print compile errors if any
		glGetShader(fragment, GL_COMPULE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			td::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		//Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShdaer(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print linking error if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//Use the program
	void use() {
		glUseProgram(this->Program);
	}

};





#endif