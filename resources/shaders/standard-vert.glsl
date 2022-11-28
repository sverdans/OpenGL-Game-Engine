#version 460
#define MaxDirectionalLights 8

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;
 
out VS_OUT 
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 LightSpace[MaxDirectionalLights];
	vec4 CameraSpace[MaxDirectionalLights];
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix[MaxDirectionalLights];

void main()
{
	vs_out.FragPos = vec3(model * vec4(vertexPosition, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;
	vs_out.TexCoords = aTexCoords;
	//for (int i = 0; i < MaxDirectionalLights; i++)
	//	vs_out.LightSpace[i] = lightSpaceMatrix[i] * model * vec4(vertexPosition, 1.0);
	vs_out.LightSpace[0] = lightSpaceMatrix[0] * model * vec4(vertexPosition, 1.0);
	vs_out.CameraSpace[0] = projection * view * model * vec4(vertexPosition, 1.0);
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}