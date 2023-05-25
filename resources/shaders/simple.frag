#version 460
uniform vec3 modelColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(modelColor, 1.0);
}