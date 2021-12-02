#pragma once
#include "Shader.h"
#include "glm/glm.hpp"

class Material {
public:
	static glm::mat4* proj, *view;
	Shader* shader;
	Material(std::string path, glm::mat4 * proj, glm::mat4* view);
	void Render();

private:
	glm::vec4 r_float();
	glm::vec4 r_vec2();
	glm::vec4 r_vec3();
	glm::vec4 r_vec4();
	const int 
};