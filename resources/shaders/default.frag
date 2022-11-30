#version 460
#define MaxDirectionalLights 8
#define MaxPointLights 8

in VertOut 
{
	vec3 fragPosition;
	vec3 normal;
	vec2 textureCoords;
} fragIn;

out vec4 fragColor;

uniform vec3 viewPosition;

uniform int haveTextures;
uniform vec3 modelColor;

uniform vec3 ambientColor;
uniform float ambientStrength;

uniform float specularStrength;
uniform int specularity;

uniform struct DirectionalLights
{
	float intensity;
	vec3 direction;
	vec3 color;
} directionalLights[MaxDirectionalLights];

uniform struct PointLights
{
	float intensity;
	vec3 position;
	vec3 color;
} pointLights[MaxPointLights];

void main()
{
	vec3 norm = normalize(fragIn.normal);

	vec3 lighting = ambientStrength * ambientColor;
	
	for (int i = 0; i < MaxPointLights; i++)
	{
		vec3 lightDir = normalize(pointLights[i].position - fragIn.fragPosition);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * pointLights[i].color;

	//	float dist = pointLights[i].intensity / length(pointLights[i].position - fragIn.fragPosition);
	//	vec3 diffuse = dist * pointLights[i].color;

		vec3 viewDir = normalize(viewPosition - fragIn.fragPosition);
		vec3 reflectDir = reflect(-lightDir, norm);  

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
		vec3 specular = specularStrength * spec * pointLights[i].color;  

		lighting += diffuse + specular;
	}

	fragColor = vec4(lighting * modelColor, 1.0);
}