#version 140

in vec2 vo_UV;

out vec4 fragmentColour;

uniform sampler2D textureSampler;
uniform vec3 Colour; 

void main()
{
	vec4 sampled = vec4(Colour, texture(textureSampler, vo_UV));
	fragmentColour = sampled;
}

