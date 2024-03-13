#version 140

in vec3 vertexPos;
in vec2 vi_UV;
in vec3 vi_Normals;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 vo_UV;
out vec3 vo_Normals;
out vec3 fragPosition;


void main()
{
	vo_UV = vi_UV;
	vec4 v = vec4(vertexPos.xyz, 1);

	// calculate MVP 
	v = projection * view * transform * v;

	// set where object is going to be in scene
	gl_Position = v;

	vo_Normals = vec3(transform * vec4(vi_Normals, 1.0f));
	fragPosition = vec3(transform * vec4(vertexPos,1.0f));
}
	