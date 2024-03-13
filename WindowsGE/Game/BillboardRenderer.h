#pragma once
#include "Camera.h"
#include "Billboard.h"
#include "ShaderUtils.h"
#include "VertexArray.h"
#include "Structs.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	class BillboardRenderer : public Object
	{
	public:
		BillboardRenderer(std::string vert_filepath, std::string frag_filepath);

		~BillboardRenderer() {
		}

		void init();

		virtual void loadVA();

		virtual void draw(Shared<Billboard>Billboard, Shared<Camera> cam);

		virtual void initShader(std::string vert_filepath, std::string frag_filepath);

	protected:
		VertexUV billboard[6] = {
		VertexUV(0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		VertexUV(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexUV(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),

		VertexUV(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
		VertexUV(0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
		VertexUV(0.5f, 1.0f, 0.0f, 1.0f, 1.0f)
		};

		unsigned int indices[6] = {
		0,1,2,
		2,4,0
		};

		// Member fields
		// This member stores the program object that contains the shaders
		GLuint shaderID;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation;

		// This member stores the triangle vertex buffer object containing the vertices
		// transferred from this code to the graphics memory
		GLuint vboQuad;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
		Shared<Shader> shader_bilboard;

		Shared<VertexBuffer> vb;
		Shared<IndexBuffer> ib;
		Shared<VertexArray> va;
		Shared<VertexBufferLayout> vl;
	};
}
