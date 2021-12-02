#pragma once

#include "glm/gtx/quaternion.hpp"
#include "Window.h"
#include "PerlinNoise.hpp"
#include "gui/Hud.h"
#include "Camera.h"

class ChaseCam : public Camera {
	const float CAM_SHAKE_X = 0.05f;
	const float CAM_SHAKE_Y = 0.05f;
	const float CAM_SHAKE_Z = 0.05f;
	const float SHAKE_SPEED = 1000.0f;
	const float SENS_X = 0.1f;
	const float SENS_Y = 0.1f;

	siv::PerlinNoise xnoise, ynoise, znoise;
	float fov;
	float distance;

	float trauma;
	//pitch and yaw, but actually (yaw, pitch)
	glm::vec2 cursor_rot;
protected:
	glm::mat4 getCamShake();
public:
	glm::vec3* target;
	glm::vec2 rot;
	glm::vec3 cursorForward;

	texInstance* cursor;

	ChaseCam(float fov, float distance, Window* win, Hud * hud);
	void setTarget(glm::vec3 * target);

	//call every frame or die
	void cam_Update() override;

	//takes the mouse deltas
	void mouseIn(double dx, double dy);
	
	//input is 0 - 2, 0 being a minor trauma and 2 being awful
	void addTrauma(int toADD);
	void setTrauma(float toSet);
};