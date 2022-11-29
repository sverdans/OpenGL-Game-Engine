#version 460
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;

out VertOut 
{
	vec3 fragPosition;
	vec3 normal;
	vec2 textureCoords;
} vertOut;

uniform mat4 viewProjectionMat;
uniform mat4 modelMat;

void main()
{
	vertOut.fragPosition = vec3(modelMat * vec4(vertexPosition, 1.0));
	vertOut.normal = transpose(inverse(mat3(modelMat))) * normal;
	vertOut.textureCoords = vec2(0.0, 0.0);

	gl_Position = viewProjectionMat * modelMat * vec4(vertexPosition, 1.0);
}