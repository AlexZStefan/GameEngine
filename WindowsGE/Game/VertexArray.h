#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Functions.h"

namespace GE {

//#define VERTEX_UV 20
	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned int normalized;

		// return type of buffer layout suported types
		static unsigned int getTypeSize(unsigned int type) {
			switch (type) {
				case GL_FLOAT: return 4; 
				case GL_UNSIGNED_INT: return 4;
				case GL_BYTE: return 1;
				case GL_FLOAT_MAT4: return 64;
				case GL_FLOAT_VEC4: return 16;

		}
			ASSERT(false);
			return 0;

		}
	};

	class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride = 0;
		
	public:
		VertexBufferLayout() {};
		~VertexBufferLayout() {};

		unsigned int layout_count = 0;
		inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
		inline unsigned int getStride() const { return m_stride; }

		template<typename T>
		void push(unsigned int count) {
			static_assert(false);
		}
		// if char is already to 255 normalized so GL_TRUE 
		template<>
		void push<float>(unsigned int count) {
			// type, count, normalized
			m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_stride += VertexBufferElement::getTypeSize(GL_FLOAT) * count;
			layout_count++;
		};

		template<>
		void push<unsigned int>(unsigned int count) {
			m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_stride += VertexBufferElement::getTypeSize(GL_UNSIGNED_INT) * count;
			layout_count++;
		};

		template<>
		// have to push each individual vec4 - this will be repeted for times in VA.cpp
		void push<glm::vec4>(unsigned int count) {
			m_elements.push_back({ GL_FLOAT_VEC4 , count, GL_FALSE });
			// mat4 can only be set as vectors into shader layer 
			m_stride += VertexBufferElement::getTypeSize(GL_FLOAT_MAT4) ;
			layout_count += 4; 
		};


	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind();
		void unbind();

		void addBuffer(const Shared<VertexBuffer>& _vb, 
			const Shared<VertexBufferLayout>& _vbLayout, bool particles, GLint _shaderID = 0);

	private:
		unsigned int offset = 0; // offset for pointer to vertex location
		unsigned int y = 0; // used as offset for additional data

		GLuint m_rendererID;
		std::vector<Shared<VertexBufferLayout>> m_vLayout;

	};


}