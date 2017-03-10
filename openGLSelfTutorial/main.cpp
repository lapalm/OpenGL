
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "SOIL.h"
#include "shaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//using namespace shader;
using namespace std;

//Global Variables


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//Closing the application
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//When user presses the escape key, we set the WindowShouldClose property to true.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main() {

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

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Build and compile our shader program
	Shader program("minimal.vert", "minimal.frag");

	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	//VBO = Vertex Buffer Object
	//VAO = Vertex Array Object
	//EBO = Element Buffer Object - EBO is a buffer, just like the vertex buffer object, that stores indices that OpenGL uses to
								  //decide what vertices to draw. This is done by storing only the unique vertices and then 
								  //specify the order at which we want to draw these vertices in. 
								  //Example: Use 4 vertices to draw a square using 2 triangles instead of 6. 

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Load and create a texture 
	GLuint texture1;
	GLuint texture2;

	// ====================
	// Texture 1
	// ====================
	/*glGenTextures() takes as input how many textures we want to generate and stores them in a 
	GLuint array given as its second argument (in our case just a single GLuint)*/
	glGenTextures(1, &texture1);

	/*we need to bind it so any subsequent texture commands will configure the currently bound texture:*/
	glBindTexture(GL_TEXTURE_2D, texture1);
	// All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width;
	int	height;
	// Load image, create texture and generate mipmaps
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	/*
	glTexImage2D
	1) The first argument specifies the texture target; setting this to GL_TEXTURE_2D means 
	this operation will generate a texture on the currently bound texture object at the 
	same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).

	2) The second argument specifies the mipmap level for which we want to create a texture 
	for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.

	3) The third argument tells OpenGL in what kind of format we want to store the texture. Our image
	has only RGB values so we'll store the texture with RGB values as well.

	4) The 4th and 5th argument sets the width and height of the resulting texture. 
	We stored those earlier when loading the image so we'll use the corresponding variables.

	5) The next argument should always be 0 (some legacy stuff).

	6) GLenum format 

	7-8) The 7th and 8th argument specify the format and datatype of the source image. 
	We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in 
	the corresponding values.

	9) The last argument is the actual image data.
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done (set second parameter to 0), so we won't accidentily mess up our texture.

	// ===================
	// Texture 2
	// ===================
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Game loop
	//Checks if GLFW has been instructed to close.
	while(!glfwWindowShouldClose(window)) {

		//Checks and calls events
		glfwPollEvents(); 


		//Rendering commands go here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Color to clear with
		glClear(GL_COLOR_BUFFER_BIT); //Clears the screen
		
		//Draw Codes go here

		/*Texture unit GL_TEXTURE0 is always by default activated.
		
		OpenGL should have a at least a minimum of 16 texture units for you 
		to use which you can activate using GL_TEXTURE0 to GL_TEXTURE15. 
		They are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for example, 
		which is useful when we'd have to loop over several texture units.
		*/

		//shader::useShaderProgram(programs["initShader"]);
		program.Use();

		//Compute Transformations
		glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // glm::radians(90.0f)
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		
		vec = trans * vec;
		cout << vec.x << vec.y << vec.z << endl;

		//Get matrix's uniform location and set matrix
		GLuint transformLoc = glGetUniformLocation(program.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glActiveTexture(GL_TEXTURE0);
		/*After activating a texture unit, a subsequent 
		glBindTexture call will bind that texture to the currently active texture unit. */
		glBindTexture(GL_TEXTURE_2D, texture1);
		/*By setting them via glUniform1i we make sure each uniform sampler corresponds to the proper texture unit.*/
		glUniform1i(glGetUniformLocation(program.Program, "ourTexture1"), 0); //programs["initShader"]
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(program.Program, "ourTexture2"), 1); //programs["initShader"]

		//Draw Container
		glBindVertexArray(VAO);
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
