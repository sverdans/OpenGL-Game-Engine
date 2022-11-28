#version 460
#define MaxDirectionalLights 8
#define MaxPointLights 16

out vec4 FragColor;
 
in VS_OUT 
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 LightSpace[MaxDirectionalLights];
	vec4 CameraSpace[MaxDirectionalLights];
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform int hasTextures;

uniform vec3 viewPos;
uniform float specularIntensity;
uniform int receiveShadows;

uniform struct DirectionalLights
{
	vec3 direction;
	float intensity;
	vec4 color;
	int useSpecular;
	sampler2D shadowMap;
} directionalLights[MaxDirectionalLights];

uniform struct PointLights
{
	vec3 position;
	float intensity;
	vec4 color;
	int useSpecular;
} pointLights[MaxPointLights];

float ShadowCalculation(vec4 lightSpace, float bias)
{
	vec3 projCoords = lightSpace.xyz / lightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(directionalLights[0].shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	if (currentDepth > 1.0)
        shadow = 0.0;

	return shadow;
}

void main()
{
	vec3 normal = normalize(fs_in.Normal);
	
	// ambient lighting
	vec4 ambient = vec4(vec3(0.1), 1);// * color;
	
	vec4 lighting = ambient;
	for (int i = 0; i < MaxDirectionalLights; i++)
	{
		// diffuse lighting
		vec3 lightDir = normalize(directionalLights[i].direction);
		float diff = max(dot(lightDir, normal), 0.0);
		vec4 diffuse = diff * directionalLights[i].color * directionalLights[i].intensity;

		// specular lighting
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
		vec4 specular = spec * directionalLights[i].color;

		lighting += diffuse + (directionalLights[i].useSpecular > 0.5 ? specular : vec4(0));
	}

	/*vec3 lightDir = normalize(directionalLights[0].direction);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor * directionalLights[0].intensity;
	float bias = 0.0002;
	float shadow = ShadowCalculation(fs_in.LightSpace[0], bias);
	lighting += diffuse * (receiveShadows > 0.5 ? (1 - shadow) : 1);*/

	for (int i = 0; i < MaxPointLights; i++)
	{
		vec3 lightDir = normalize(pointLights[i].position - fs_in.FragPos);

		// diffuse lighting
		float dist = pointLights[i].intensity / length(pointLights[i].position - fs_in.FragPos);
		vec4 diffuse = dist * pointLights[i].color;

		// specular lighting
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
		vec4 specular = spec * pointLights[i].color;

		lighting += diffuse + (pointLights[i].useSpecular > 0.5 ? specular : vec4(0));
	}
	
	// final lighting
	FragColor = lighting * (hasTextures > 0.5 ? texture(texture_diffuse1, fs_in.TexCoords) : vec4(1));
}