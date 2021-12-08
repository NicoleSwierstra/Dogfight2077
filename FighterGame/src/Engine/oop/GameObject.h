#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <string>
#include "GameObject.h"

class Scene;

class GameObject {
protected:
	void updateTrans();
	glm::mat4 trans;
public:
	std::string name;
	Scene* scene;
	glm::vec3 pos;
	glm::quat rot;

	virtual void Render() = 0;
	virtual void Update() = 0;
};