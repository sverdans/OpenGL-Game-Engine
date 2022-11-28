#version 460
in vec3 color;
out vec4 fragColor;

uniform int haveTextures;
uniform vec3 modelColor;

void main()
{
	fragColor = vec4(modelColor, 1.0);
}