#version 460
#define MaxDirectionalLights 8
#define MaxPointLights 8

in VertOut 
{
	vec3 normal;
	vec3 fragPosition;
	vec2 textureCoords;
} fragIn;

out vec4 fragColor;

uniform vec3 viewPosition;

uniform int useTexture;
uniform vec3 modelColor;

uniform sampler2D diffuseTexture1;
uniform sampler2D specularTexture1;

uniform vec3 materialColor;

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
		vec3 diffuse = diff * pointLights[i].color * pointLights[i].intensity;

	//	float dist = pointLights[i].intensity / length(lightDir);
	//	vec3 diffuse = dist * pointLights[i].color;

		vec3 viewDir = normalize(viewPosition - fragIn.fragPosition);
		vec3 reflectDir = reflect(-lightDir, norm);  
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float spec = pow(max(dot(norm, halfwayDir), 0.0), specularity);
		vec3 specular = spec * pointLights[i].color;

		lighting += diffuse + specular;
	}

	if (useTexture > 0.5)
	//	fragColor = vec4(lighting, 1.0) * texture(diffuseTexture1, fragIn.textureCoords);
		fragColor = vec4(lighting * materialColor, 1.0);
	else
		fragColor = vec4(lighting * modelColor, 1.0);
}