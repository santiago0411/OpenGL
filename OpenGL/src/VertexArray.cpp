#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& [Type, Count, Normalized] = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, Count, Type, Normalized, layout.GetStride(), (const void*)offset));
		offset += Count * VertexBufferElement::GetSizeOfType(Type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
