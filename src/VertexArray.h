#pragma once

#include"VertexBuffer.h"
#include"VertexBufferLayout.h"

class VertexArray {
private:
	 
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& VB, const VertexBufferLayout& layout);
};