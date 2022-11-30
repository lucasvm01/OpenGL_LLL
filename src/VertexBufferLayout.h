#pragma once

#include<vector>
#include<GLAD/glad.h>

struct VertexBufferElement {
	unsigned int  type;
	unsigned int count;
	unsigned char isNormalized;
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() :
		m_Stride(0) {}

	template<typename T>
	void Push(int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(int count) {
		m_Elements.push_back(
			{ GL_FLOAT, (unsigned int) count, GL_FALSE });
		m_Stride += sizeof(GLfloat);
	}

	template<>
	void Push<unsigned char>(int count) {
		m_Elements.push_back(
			{ GL_UNSIGNED_BYTE, (unsigned int) count, GL_TRUE });
		m_Stride += sizeof(GLubyte);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};