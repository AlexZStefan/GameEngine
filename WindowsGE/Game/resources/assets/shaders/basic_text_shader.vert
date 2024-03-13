
#version 140

in vec3 vertexPos;
in vec2 vi_UV;

uniform mat4 projection;

out vec2 vo_UV;

void main()
{
	vo_UV = vi_UV;

	vec4 v = projection * vec4(vertexPos.xyz, 1.0f);
	gl_Position = v;
	vo_UV = vi_UV;
}
	