#version 140

// Inputs to vertex shader
in vec3 vertexPos;
in vec2 vUV;
in mat4 instanceMatrix;
// Output
out vec2 uv;

uniform mat4 view;
uniform mat4 projection;

// Shader main code
void main() {
	// Convert input vertex to 4D 
	vec4 v = vec4(vertexPos.xyz, 1);
	 
	//vec4 crntPos = instanceMatrix * vec4(vertexPos, 1.0);

	// Transform
	v = projection * view * instanceMatrix * v;

	// Pass transformed vertex to next stage
	gl_Position = v;

	// As well as uv.  Don't transform uv
	uv = vUV;
}