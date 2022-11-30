#include"VertexBuffer.h"

#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
	// Generate and bind a Vertex Buffer Object and copy the coords to it
	// It is not possible to do this step if there is no VAO bound
	// "Create a buffer, select that buffer (like a layer in photoshop) and copy stuff to it"
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer(){
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
