#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>

using namespace std;

	namespace shader {
		GLuint initShaders(const char *vertFile, const char *fragFile);
		GLchar* loadFile(const char *fname, GLint &fSize);
	}
	
#endif