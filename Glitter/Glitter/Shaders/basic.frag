#version 330 core
in vec2 TexCoord;
out vec4 ColorOut;

uniform sampler2D texture1;

void main()
{
    ColorOut = texture(texture1, TexCoord);
}