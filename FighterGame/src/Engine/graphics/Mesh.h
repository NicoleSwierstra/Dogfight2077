#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Vertex.h"

#define UNIFORM_FLOAT 0x0
#define UNIFORM_INT 0x1
#define UNIFORM_MAT4 0x2
#define UNIFORM_VEC2 0x3
#define UNIFORM_VEC3 0x4
#define UNIFORM_VEC4 0x5

struct uniformNode {
	std::string name;
	void* data;
	int type;
};

struct shaderNode {
	Shader* shader;
	std::vector<uniformNode> uniforms;
	int meshId;
	int type;
	bool wireframe;
};

struct submesh {
	VertexBuffer* vb;
	std::vector<vertex> verts;
};

class Mesh {
public:
	std::vector<submesh> sm;
	std::vector<shaderNode> sh;
	void LoadObj(const char* filepath);
	~Mesh();
	Mesh(const std::string& filepath);
	Mesh(std::vector<glm::vec3>&, std::vector<unsigned int>&);

	Mesh* AddShader(Shader* s, std::vector<uniformNode> un, int meshId, int type, bool wireframe);
	Mesh* AddShader(Shader* s, glm::mat4* proj, glm::mat4* view, std::vector<uniformNode> un, int meshId, int type, bool wireframe);
	void Optimise();

	void applyUniform(uniformNode* uniform, Shader* shader);
	void applyUniforms(shaderNode* shader);

	void Render();
	void RenderMultiple(std::vector<glm::vec3>& positions, std::vector<uniformNode> uniforms, const char* uniform);
};