#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f)); //Transform the vertex Position with the model matrix to get the FragPos in world coords.
	// the transpose of the inverse of the upper-left corner of the model matrix : see http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
	Normal = normal; // mat3(transpose(inverse(model))) * normal <- use if you want to scale, but try to do in CPU instead. Less expensive
	TexCoords = texCoords;
} 