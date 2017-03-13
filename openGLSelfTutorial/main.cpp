
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

//Light Attribute
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile our shader program
	//Shader program("minimal.vert", "minimal.frag");
	Shader lightingShader("lighting.vert", "lighting.frag");
	Shader lampShader("lamp.vert", "lamp.frag");

	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//VBO = Vertex Buffer Object
	//VAO = Vertex Array Object
	//EBO = Element Buffer Object - EBO is a buffer, just like the vertex buffer object, that stores indices that OpenGL uses to
								  //decide what vertices to draw. This is done by storing only the unique vertices and then 
								  //specify the order at which we want to draw these vertices in. 
								  //Example: Use 4 vertices to draw a square using 2 triangles instead of 6. 

	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Tex attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// ===================
	// CREATE LAMP
	// ===================
	//Set the lights VAO only as the vertices are the same for the light object (also a 3D cube)
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//We only need ot bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Set the vertex attributes (only position data for our lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	/*Texture unit GL_TEXTURE0 is always by default activated.

	OpenGL should have a at least a minimum of 16 texture units for you
	to use which you can activate using GL_TEXTURE0 to GL_TEXTURE15.
	They are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for example,
	which is useful when we'd have to loop over several texture units.
	*/

	// Load textures
	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);
	int width, height;
	unsigned char* image;

	// Diffuse map
	image = SOIL_load_image("container2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Specular map
	image = SOIL_load_image("container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Emission map
	image = SOIL_load_image("matrix.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.emission"), 2);

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Color to clear with
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the screen
		
		//Draw Codes go here

		// Change the lights positon value over time. 
		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		lightingShader.Use();

		//LightPosi + View Pos
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		//Set Light properties
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		/*glm::vec3 lightColor;
		lightColor.x = sin((GLfloat)(glfwGetTime()*0.2) * 2.0f);
		lightColor.y = sin((GLfloat)(glfwGetTime()*0.2) * 0.7f);
		lightColor.z = sin((GLfloat)(glfwGetTime()*0.2) * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); //Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "light.specular");

		glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z); // Normally set light color to white.
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);*/

		//Set Material Properties

		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f);

		//Create Camera Transformation
		//View:
		glm::mat4 view = camera.GetViewMatrix();

		//Projection:
		//We want to use perspective projection for our scene so we'll declare the projection matrix like this:
		//glm::perspective (FOV, Aspect Ratio, zNear, zFar)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		//Get the uniform location 
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		//Pass to shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind Diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		//Bind Specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//Bind Emission map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		//Draw Container
		glBindVertexArray(containerVAO);
		glm::mat4 model;

		//Pass to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Draw Lamp 
		lampShader.Use();

		//Get the uniform location 
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Set Matrices
		//Pass to shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::rotate(model, (GLfloat)(glm::radians(45.0f) * glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f)); //Scale down lamp to make a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Swap the buffers
		glfwSwapBuffers(window);
	}

	//Properly de-allocate all resources once they've outlived their purpose
	//A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. This means it stores its unbind calls
	//so make sure you don't unbind the element array buffer before unbinding your VAO, otherwise it doesn't have an EBO
	//configured. 
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Terminate & Clean up resources before application exit
	glfwTerminate();
	return 0;
}

