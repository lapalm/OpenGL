
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "SOIL.h"
#include "shaderProgram.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//using namespace shader;
using namespace std;

//Global Variables

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//Camera Position
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Yaw = x axis view
GLfloat yaw = -90.0f; //Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector 
					  // pointing to the right(due to how Eular angles work) so we initially rotate a bit to the left.
//Pitch = y axis view
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
GLfloat fov = 45.0f;

bool keys[1024];

//Timestep
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


//Closing the application
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Mouse control

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	//Check if this is the first time receiving the mous input and if so, we first update the initial mouse position
	// to the new xpos and ypos values; the resuling mouse movement will then use the entered mouse's position
	// coordinates to calculate it's offset. - Helps avoid large offsets and movement jump when mouse enters the 
	// program display for the first time. 
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//Calcualate the offset movement between the last and current frames.
	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos; //Reverse since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


void do_movement() {
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

	// Set the required callback functions to register
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader program("minimal.vert", "minimal.frag");

	GLfloat vertices[] = {
		// Positions        // Texture Coords

		- 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Checks and calls events
		glfwPollEvents(); 
		do_movement();

		//Rendering commands go here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Color to clear with
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the screen
		
		glActiveTexture(GL_TEXTURE0);
		/*After activating a texture unit, a subsequent
		glBindTexture call will bind that texture to the currently active texture unit. */
		glBindTexture(GL_TEXTURE_2D, texture1);
		/*By setting them via glUniform1i we make sure each uniform sampler corresponds to the proper texture unit.*/
		glUniform1i(glGetUniformLocation(program.Program, "ourTexture1"), 0); //programs["initShader"]
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(program.Program, "ourTexture2"), 1); //programs["initShader"]
		//Draw Codes go here

		/*Texture unit GL_TEXTURE0 is always by default activated.
		
		OpenGL should have a at least a minimum of 16 texture units for you 
		to use which you can activate using GL_TEXTURE0 to GL_TEXTURE15. 
		They are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for example, 
		which is useful when we'd have to loop over several texture units.
		*/

		program.Use();

		//Camera View & Transformation

		GLfloat radius = 25.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		
		//View:
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Compute Transformations

		//Model:
		glm::mat4 model;
		
		//By multiplying the vertex coordinates with this model 
		//matrix we're transforming the vertex coordinates to world coordinates. 
		model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians (-55.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		//Note that we're translating the scene in the reverse direction of where we want to move.
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

		//Projection:
		glm::mat4 projection;

		//We want to use perspective projection for our scene so we'll declare the projection matrix like this:
		//glm::perspective (FOV, Aspect Ratio, zNear, zFar)
		projection = glm::perspective(glm::radians(camera.Zoom), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		//Get the uniform location 
		GLint modelLoc = glGetUniformLocation(program.Program, "model");
		GLint viewLoc = glGetUniformLocation(program.Program, "view");
		GLint projLoc = glGetUniformLocation(program.Program, "projection");
		

		//Draw Container
		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = glm::radians(20.0f) * i;

			// Every 3rd iteration(including the first) we set the angle using GLFW's time function.

			if (i % 1 == 0) {
				angle = (GLfloat)glfwGetTime() * glm::radians(25.0f);
			}
			model = glm::rotate(model,  angle * (i + 1), glm::vec3(1.0f, 0.3f, 0.5f));

			//Pass to shader
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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

