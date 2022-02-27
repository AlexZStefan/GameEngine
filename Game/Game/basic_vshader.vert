
#version 140

in vec3 vertexPos;
in vec2 vi_UV;
out vec2 vo_UV;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vo_UV = vi_UV;
	vec4 v = vec4(vertexPos.xyz, 1);
	v = projection * view * transform * v;
	gl_Position = v;
}
	