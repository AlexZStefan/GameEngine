	

#version 140

in vec2 vo_UV;
uniform sampler2D sampler;
out vec4 fragmentColour;

void main()
{
	fragmentColour = texture(sampler, vo_UV).rgba;
}

