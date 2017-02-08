// Fragment Shader – file "minimal.frag"

#version 330 core

out vec4 color;

void main(){
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

//The fragment shader only requires one output variable and that is a vector of size 4 that defines the final color output that
//we should calculate ourselves. We can declare output values with the out keyword, that we here promptly named color. 