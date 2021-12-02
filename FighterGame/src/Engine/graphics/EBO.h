#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define EBO_QUAD 0x0

class EBO {
private:
	unsigned int bufferId;
	void makequad();
public:
	int elements;
	EBO(const void* data, unsigned int size);
	EBO(int type); //0 = quad
	~EBO();

	void Bind() const;
	void UnBind() const;
	void Write(const void* data, unsigned int size);
};