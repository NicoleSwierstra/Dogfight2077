#pragma once
#include "glm/glm.hpp"
#include "graphics/Shader.h"
#include "graphics/VertexBuffer.h"

//for like actual shooting not the physics
struct __BULLET_STRUCT {
	glm::vec3 origin;
	float time0;
	glm::vec3 direction;
	float time;
};

class bulletHandler {
	static bulletHandler* bh;
	Shader* shader;
	VertexBuffer* vb;

	std::vector<__BULLET_STRUCT> bullets;
public:
	static void Init();

	static void addBullet(glm::vec3 start, glm::vec3 dir);
	static void Render(glm::mat4 proj, glm::mat4 view);
};