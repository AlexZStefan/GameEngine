#shader vertex
#version 140
in vec2 vertexPos2D;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	vec4 v = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);
	v = projection * view * transform * v;
	gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);
}
#shader fragment
#version 140
out vec4 fragmentColour;
void main()
{
	fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);
}