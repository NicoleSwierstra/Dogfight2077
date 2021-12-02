#pragma once
#include "oop/GameObject.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/EBO.h"

class GraphicsSun : public GameObject {
	VertexBuffer** vb; //3 vertex buffers
	EBO* ebo;
	Shader* sunShader;
public:
	GraphicsSun();

	void Update() override;
	void Render() override;
};