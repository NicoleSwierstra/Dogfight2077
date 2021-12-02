#include "EBO.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void EBO::makequad() {
    unsigned int ebo[6] = {
        0, 1, 2,
        1, 2, 3
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), ebo, GL_STATIC_DRAW);
    elements = 6;
}

EBO::EBO(const void* data, unsigned int size)
{
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    elements = size/sizeof(int);
}

EBO::EBO(int type)
{
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    switch (type) {
    case EBO_QUAD:
        makequad();
        break;
    }
}

EBO::~EBO()
{
    glDeleteBuffers(1, &bufferId);
}

void EBO::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
}

void EBO::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Write(const void* data, unsigned int size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    elements = size / sizeof(int);
}