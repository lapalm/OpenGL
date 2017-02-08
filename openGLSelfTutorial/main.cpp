#include <GL\glew.h>
#include "utilityHelper.h"
#include <GLFW\glfw3.h>
#include <iostream>

using namespace uHelper;
using namespace std;


//Closing the application
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//When user presses the escape key, we set the WindowShouldClose property to true.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main() {
	int width = 0;
	int	height = 0;

	glfwInit();

	//Set OpenGL version to (Major, Minor). 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Explicitly tell OpenGL not to use Legacy Functionality.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	//Resize Window Option
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	//Create Window
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "Penguine", nullptr, nullptr);

	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	} 
	glfwMakeContextCurrent(window);

	//Set Key callback
	glfwSetKeyCallback(window, key_callback);

	//Ensures GLEW uses more modern techniques for managing OpenGL functionality. 
	glewExperimental = GL_TRUE; // Default GL_FALSE might give issues when using the core profile of OpenGL.

	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glfwGetFramebufferSize(window, &width, &height);

	//Set Viewport
	//First two parameters sets the location of the lower left corner of the window. 
	//Third and fourth parameter sets the width and heigh of the rendering window in pixels. 
	//We receive the width and height from GLFW itself so it also works on high DPI screens. 
	glViewport(0, 0, width, height);

	//Create shader object and compile the shader.
	// Vertex shader
	GLint vLen;
	GLuint vertexShader;
	const char* vertexShaderSource = loadFile("minimal.vert", vLen);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, &vLen);
	glCompileShader(vertexShader);
	
	//Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	
	//Fragment shader
	GLint fLen;
	GLuint fragmentShader;
	const char* fragmentShaderSource = loadFile("minimal.frag", fLen);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, &fLen);
	glCompileShader(fragmentShader);

	//Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//Linking Shaders (Shader program)
	//The glCreateProgram function creates a program and returns the ID reference to the newly created program object.
	//Now we need to attach the previously compiled shaders to the program object and then link them with glLinkProgram.
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	//Check for compile time errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::LINK::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//Don't forget to delete the shader objects once we've linked them into the program object; we no longer need them anymore.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] = {
		 0.5f, 0.5f, 0.0f, // Top Right
		 0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f, // Top Left
	};

	GLuint indices[] = { // Note: we start from 0!
		0, 1, 3, //First triangle
		1, 2, 3  //Second Triangle
	};

	//VBO = Vertex Buffer Object
	//VAO = Vertex Array Object
	//EBO = Element Buffer Object - EBO is a buffer, just like the vertex buffer object, that stores indices that OpenGL uses to
								  //decide what vertices to draw. This is done by storing only the unique vertices and then 
								  //specify the order at which we want to draw these vertices in. 
								  //Example: Use 4 vertices to draw a square using 2 triangles instead of 6. 

	GLuint VBO, VAO, EBO;

	//Bind Vertex Array Object
	//To use the VAO all you have to do is bind the VAO using glBindVertexArray.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	//Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Note: This is allowed, the call to glVertexAttribPointer registered VBO as the currently bound
	//vertex buffer object so afterwards we can safely unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	//Unbind the VAO (NOT the EBO)
	glBindVertexArray(0);

	//Wireframe Polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Game loop
	//Checks if GLFW has been instructed to close.
	while(!glfwWindowShouldClose(window)) {

		//Checks and calls events
		glfwPollEvents(); 


		//Rendering commands go here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Color to clear with
		glClear(GL_COLOR_BUFFER_BIT); //Clears the screen
		
		//Draw the first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Swap the buffers
		glfwSwapBuffers(window);
	}

	//Properly de-allocate all resources once they've outlived their purpose
	//A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. This means it stores its unbind calls
	//so make sure you don't unbind the element array buffer before unbinding your VAO, otherwise it doesn't have an EBO
	//configured. 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//Terminate & Clean up resources before application exit
	glfwTerminate();
	return 0;
}
