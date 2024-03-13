#include "GUIRenderer.h"

namespace GE {

	GUIRenderer::GUIRenderer(int _width, int _height) : window_width(_width), window_height(_height) {
		glGenBuffers(1, &vboTextQuad);

		// create empty buffer
		vb = std::make_shared<VertexBuffer>(6 * 5 * sizeof(float), nullptr, 2);
		vl = std::make_shared<VertexBufferLayout>();
		va = std::make_shared<VertexArray>();

		vl->push<float>(3);
		vl->push<float>(2);
		va->addBuffer(vb, vl, 0);

	};

	void GE::GUIRenderer::drawText(int xPos, int yPos, FontRenderer* _font)
	{
		int w = _font->getWidth();
		int h = _font->getHeight();

		textQuad[0] = VertexUV(xPos, yPos, 0.0f, 0.0f, 1.0f);
		textQuad[1] = VertexUV(xPos, yPos + h, 0.0f, 0.0f, 0.0f);
		textQuad[2] = VertexUV(xPos + w, yPos + h, 0.0f, 1.0f, 0.0f);

		textQuad[3] = VertexUV(xPos + w, yPos + h, 0.0f, 1.0f, 0.0f);
		textQuad[4] = VertexUV(xPos + w, yPos, 0.0f, 1.0f, 1.0f);
		textQuad[5] = VertexUV(xPos, yPos, 0.0f, 0.0f, 1.0f);

		glUseProgram(m_ShaderID);
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// set the buffer data dynamicly 
		vb->bind();
		// 0 is the offset of from where we want this data to start
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textQuad), &textQuad);
		va->bind();

		glActiveTexture(GL_TEXTURE0);
		//glUniform1i(m_ShaderID, 0);
		glBindTexture(GL_TEXTURE_2D, _font->getFont());

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
		va->unbind();
	}

	void GE::GUIRenderer::addShader(Shared<Shader> _shader)
	{
		m_ShaderID = _shader->getProgramId();

		vertexPosLocation = glGetAttribLocation(m_ShaderID, "vertexPos");
		if (vertexPosLocation == -1) {
			std::cerr << "Problem getting vertexPos" << std::endl;
		}

		vertexUVLocation = glGetAttribLocation(m_ShaderID, "vi_UV");
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vertex UV" << std::endl;
		}

		projectionUniformId = glGetUniformLocation(m_ShaderID, "projection");
		projectionMat = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f);
	}

	void GE::GUIRenderer::drawTexture(int xPos, int yPos, Shared<Texture>& texture)
	{
		bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

		int w = texture->getWidth();
		int h = texture->getHeight();

		glDisable(GL_DEPTH_TEST);

		textQuad[0] = VertexUV(xPos, yPos, 0.0f, 0.0f, 1.0f);
		textQuad[1] = VertexUV(xPos, yPos + h, 0.0f, 0.0f, 0.0f);
		textQuad[2] = VertexUV(xPos + w, yPos + h, 0.0f, 1.0f, 0.0f);

		textQuad[3] = VertexUV(xPos + w, yPos + h, 0.0f, 1.0f, 0.0f);
		textQuad[4] = VertexUV(xPos + w, yPos, 0.0f, 1.0f, 1.0f);
		textQuad[5] = VertexUV(xPos, yPos, 0.0f, 0.0f, 1.0f);

		glUseProgram(m_ShaderID);
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// set the buffer data dynamicly 
		vb->bind();
		// 0 is the offset of from where we want this data to start
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textQuad), &textQuad);
		va->bind();

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_ShaderID, 0);

		glBindTexture(GL_TEXTURE_2D, texture->getTexture());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}


	}

}