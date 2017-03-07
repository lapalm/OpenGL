#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

void main()
{
<<<<<<< HEAD
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
=======
    gl_Position = vec4(position, 1.0f);
>>>>>>> 5cb9cdb9320720e13434c1a4f45c5b9f08f6d07f
    ourColor = color;
}