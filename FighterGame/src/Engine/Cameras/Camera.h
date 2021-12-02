#pragma once
#include <glm/glm.hpp>
#include "Window.h"

class Camera {
protected:
	Window* window;
public:
	glm::mat4 proj, view;
	virtual void cam_Update() = 0;
};