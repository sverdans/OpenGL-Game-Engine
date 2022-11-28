#version 460
layout(location = 0) in vec3 vertexPosition;

uniform mat4 viewProjectionMat;
uniform mat4 modelMat;

void main()
{
	gl_Position = viewProjectionMat * modelMat * vec4(vertexPosition, 1.0);
}