#include "VertexArray.h"

namespace GE {
	void VertexArray::addBuffer(const Shared<VertexBuffer>& _vb, const Shared<VertexBufferLayout>& _vbLayout,
		bool particles, GLint _shaderID)
	{
		m_vLayout.push_back(_vbLayout);
		glUseProgram(_shaderID);

		_vb->bind();
		bind();

		const auto& elements = _vbLayout->getElements();
		int c = 0; // particle offset for matrix multiplication

		unsigned int stride = _vbLayout->getStride();

		// loop through layout elements and set attribute of vertex acording to layout data
		// Enable the attribute to be passed vertices from the vertex buffer object
		
		// only for particles that that use a 4x4 matrix
		if (particles == true)
		{
			GLint matLoc = glGetAttribLocation(_shaderID, "instanceMatrix");

				for (unsigned int i = 0; i < elements.size(); i++) {
					const auto& e = elements[i];
					
					int atribLocation = i + matLoc;

					GLCALL(glEnableVertexAttribArray(i));
					GLCALL(glVertexAttribPointer(atribLocation, e.count, e.type, e.normalized, stride, (void*)(i*sizeof(glm::vec4))));

					glVertexAttribDivisor(atribLocation, 1);

					// y - keeps track of assigned layers
				}
		}
		else
		{
			for (unsigned int i = 0; i < elements.size(); i++) {
				const auto& e = elements[i];

				GLCALL(glEnableVertexAttribArray(i));
				GLCALL(glVertexAttribPointer(i, e.count, e.type, e.normalized, stride, (void*)offset));

				offset += e.count * VertexBufferElement::getTypeSize(e.type);
				GLCALL(glDisableVertexAttribArray(i));

				// y - keeps track of assigned layers
			}
		}
		unbind();
		_vb->unbind();
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void VertexArray::bind()
	{
		int ost = 0;

		GLCALL(glBindVertexArray(m_rendererID));

		for (int y = 0; y < m_vLayout.size(); y++) {
			int size = m_vLayout[y]->getElements().size();

			for (unsigned int i = 0; i < size + ost; i++)
			{
				glEnableVertexAttribArray(i);
			}
				ost += size;
		}
	}
	

	void VertexArray::unbind()
	{
		int ost = 0;

		GLCALL(glBindVertexArray(m_rendererID));
		for (int y = 0; y < m_vLayout.size(); y++) {
			int size = m_vLayout[y]->getElements().size();

			for (unsigned int i = 0; i < size; i++)
			{
				glDisableVertexAttribArray(i + ost);
			}
			ost += size;

		}
		GLCALL(glBindVertexArray(0));
	}

}