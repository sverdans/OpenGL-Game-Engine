#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
	GLuint id;
	unsigned int count;
public:
	IndexBuffer() : id(0) { }
	IndexBuffer(const void* data, const unsigned int count) : count(count)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	~IndexBuffer() { glDeleteBuffers(1, &id); }

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&& indexBuffer) noexcept : id(indexBuffer.id), count(indexBuffer.count)
	{
		indexBuffer.id = 0;
		indexBuffer.count = 0;
	}
	IndexBuffer& operator = (IndexBuffer&& indexBuffer) noexcept
	{
		id = indexBuffer.id;
		count = indexBuffer.count;
		indexBuffer.id = 0;
		indexBuffer.count = 0;
		return *this;
	}

	void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

	static void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	unsigned int getCount() const { return count; }
};
