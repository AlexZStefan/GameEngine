#include "BillboardRenderer.h"

namespace GE {
	// Represents a vertex in the engine
	// This is a copy of the model vertex so is unnecessary
	// duplication.  Think about how you can combine the similar
	// renderers into one type of scene object

	// Billboard quad.  Quad is a square so use scale
	// to get correct dimensions.  Is on 0,0,0

	BillboardRenderer::BillboardRenderer(std::string vert_filepath, std::string frag_filepath)
	{
		GLuint programId = 0;
	
		GLint vertexLocation = 0;
	
		GLint vertexUVLocation = 0;

		GLuint vboQuad = 0;
			
		GLuint transformUniformId = 0;
		GLuint viewUniformId = 0;
		GLuint projectionUniformId = 0;
		GLuint samplerId = 0;

		initShader(vert_filepath,  frag_filepath);

		init();
		loadVA();
	}

	void BillboardRenderer::init()
	{
		//initialize shader program
	

		shaderID = shader_bilboard->getProgramId();
		// Now get a link to the vertexPos3D so we can link the attribute
		// to our vertices when rendering
		vertexLocation = glGetAttribLocation(shaderID, "vertexPos");

		// Check for errors
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}
	
		// Now link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(shaderID, "vUV");

		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Link the uniforms to the member fields
		transformUniformId = glGetUniformLocation(shaderID, "transform");
		viewUniformId = glGetUniformLocation(shaderID, "view");
		projectionUniformId = glGetUniformLocation(shaderID, "projection");
		samplerId = glGetUniformLocation(shaderID, "sampler");

		
		
		// Lot of duplication going on here, so think about how you could reduce this
		// duplication.  Don't forget, billboard is a quad so doesn't need a model to
		// be loaded from a file
	}

	void BillboardRenderer::loadVA()
	{
		// Create the vertex buffer for the quad
		// elements to draw * vertex size amount * type size
		vb = std::make_shared<VertexBuffer>(6 * 5 * sizeof(float), &billboard);
		vl = std::make_shared<VertexBufferLayout>();
		vl->push<float>(3);
		vl->push<float>(2);
		ib = std::make_shared<IndexBuffer>(6, &indices);
		va = std::make_shared<VertexArray>();
		va->addBuffer(vb, vl,0);

		va->unbind();
		vb->unbind();
		ib->unbind();
	}

	void BillboardRenderer::draw(Shared<Billboard> b, Shared<Camera> cam)
	{
		// activate shader
		glUseProgram(shaderID);
	
		/*bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_TEST);*/

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Calculate the transformation matrix for the object.  Start with the identity matrix
		glm::mat4 transformationMat = glm::mat4(1.0f);

		// Position in world
		transformationMat = glm::translate(transformationMat, glm::vec3(b->transform.position.x , b->transform.position.y, b->transform.position.z));

		// Rotation with respect to the camera
		transformationMat = glm::rotate(transformationMat,
			glm::radians(360.0f - cam->getYaw() -90.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		// Scale, only need x and y
		transformationMat = glm::scale(transformationMat, glm::vec3(b->transform.scale.x, b->transform.scale.y, 0.0f));

		// Get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		va->bind();
		ib->bind();
		glActiveTexture(GL_TEXTURE0);
		 
		glUniform1i(samplerId, 0);

		glBindTexture(GL_TEXTURE_2D, b->getTexture()->getTexture());

		// Draw the model
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		// Unselect the attribute from the context
		glActiveTexture(0);

		ib->unbind();
		va->unbind();

		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

	/*	if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}*/
	}

	void BillboardRenderer::initShader(std::string vert_filepath, std::string frag_filepath)
	{
		shader_bilboard = std::make_shared<Shader>();
		shader_bilboard->loadShader(vert_filepath, frag_filepath);
		shader_bilboard->bind();
	}

}