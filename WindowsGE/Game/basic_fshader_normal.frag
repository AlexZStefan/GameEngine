	

#version 140

in vec2 vo_UV;
in vec3 vo_Normals;
in vec3 fragPosition;

out vec4 fragmentColour;

uniform sampler2D textureSampler;
uniform sampler2D normal_map;

uniform vec3 lightColour;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
	vec3 normal =  2.0f * texture(normal_map, vo_UV).rgb -1 ;
	normal = normalize(normal );
	//normal = normalize(normal);
	
	// converts to rgba vec4
	float ambientStrenght = 1.0f; 
	vec3 ambient = ambientStrenght * lightColour;

	// Diffuse light
	//The Diffuse lighting maximises as the dot product between the surface normal and the
	//light direction increases
		
	vec3 lightDirection = normalize( fragPosition-lightPosition);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColour;

	// Specular light
	//The Specular lighting computes the reflection vector based on the position of the fragment
	//and the viewer

	float specularStrenght = .005f;
	vec3 viewDir = normalize(viewPosition-fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), 32);
	vec3 specular = specularStrenght * spec * lightColour;

	vec3 textureColour = texture(textureSampler, vo_UV).xyz;

	vec3 result = (ambient + diffuse) * textureColour + specular;

	fragmentColour = vec4(result, 1.0f);
}

