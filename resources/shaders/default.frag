#version 460

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

uniform float lightIntensity;
uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
	vec3 norm = normalize(fragIn.normal);
	vec3 lightDir = normalize(lightPosition - fragIn.fragPosition);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPosition - fragIn.fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
    vec3 specular = specularStrength * spec * lightColor;  

	vec3 ambient = ambientStrength * ambientColor;
	vec3 result = (ambient + diffuse + specular) * modelColor;

	fragColor = vec4(result, 1.0);
}