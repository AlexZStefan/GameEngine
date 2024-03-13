#pragma once
#include "FontRenderer.h"
#include "Renderer.h"

namespace GE {
	//VertexUV;

	class GUIRenderer : public Object
	{

	public:
		// size set in fontRenderer loadFunction - position by mentioning the x and y pos
		GUIRenderer(int _width, int _height);

		~GUIRenderer() {};

		void drawText(int x, int y, FontRenderer* _font);
		void drawTexture(int xPos, int yPos, Shared<Texture>& texture);

		void addShader(Shared<Shader> _shader);
		//void  drawImage(GUIText*)

	private:
		int window_width;
		int window_height;
		unsigned int m_ShaderID;

		GLint vertexPosLocation;
		GLint vertexUVLocation;
		GLuint projectionUniformId;
		GLuint vboTextQuad;

		VertexUV textQuad[6];
		glm::mat4 projectionMat;

		Shared<VertexBuffer> vb;
		Shared<IndexBuffer> ib;
		Shared<VertexArray> va;
		Shared<VertexBufferLayout> vl;

	};

}
