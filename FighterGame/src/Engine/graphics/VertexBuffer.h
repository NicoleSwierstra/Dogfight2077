#pragma once

class VertexBuffer {
private:
	unsigned int bufferId;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
	void Write(const void* data, unsigned int size);
};