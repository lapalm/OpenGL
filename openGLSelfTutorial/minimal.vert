// Vertex Shader – file "minimal.vert"

//Version 330 corresponds to OpenGL version 3.3
#version 330 core

//Each vertext has a 3D coordinate so we create a vec3 input variable with name position
//Specifically set the location of the input variable via layout to 0. 
layout (location = 0) in vec3 position;

void main(){
//To set the output of the vertex shader, we have to assign the position data to the predefined gl_Position variable which is
//a vec4 behind the scenes. At the end of the main function, whatever we set gl_position to will be used as the output of
//the vertex shader.

//Since our input is a vector of size 3 we have to cast this to a vector of size 4. We can do this by inserting the vec3
// values inside the constructor of vec4 and set its w component to 1.0f.
gl_Position = vec4(position.x, position.y, position.z, 1.0);
}