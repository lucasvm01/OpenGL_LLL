#include"Renderer.h"

#include"VertexArray.h"

VertexArray::VertexArray(){

}

VertexArray::~VertexArray(){
}

void VertexArray::AddBuffer(const VertexBuffer& VB, const VertexBufferLayout& layout)
{
	VB.Bind();
	const auto& elements = layout.GetElements();

	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		unsigned int offset = 0;

		// Define and enable an attribute to the vertex (position - x, y - and others)
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride(), (const void*) offset));

		offset += element.count;
	}
}
