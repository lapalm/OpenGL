#ifndef utilityHelper
#define utilityHelper

#include <GL\glew.h>

#include <GLFW\glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

namespace uHelper {
	GLchar* loadFile(const char *fname, GLint &fSize);
}

#endif