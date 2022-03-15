#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 persp;
uniform mat4 transform; 

void main()
{
	TexCoord = aTexCoord;
	gl_Position = persp * transform * vec4(aPos, 1.0);
}