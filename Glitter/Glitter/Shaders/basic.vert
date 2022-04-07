#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 persp;
uniform mat4 transform;
uniform mat4 view;

void main()
{
	FragPos = vec3(transform * vec4(aPos, 1.0));
	TexCoord = aTexCoord;
	Normal = aNormal;
	gl_Position = persp * view * transform * vec4(aPos, 1.0);
}