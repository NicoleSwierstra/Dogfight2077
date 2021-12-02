#include "ChaseCam.h"
#include "util/Time.h"
#include "Input/Handler.h"

glm::mat4 ChaseCam::getCamShake()
{
	float magnitude = trauma * trauma;
	float x = CAM_SHAKE_X * xnoise.noise1D(Time::elapsed * SHAKE_SPEED * magnitude);
	float y = CAM_SHAKE_Y * ynoise.noise1D(Time::elapsed * SHAKE_SPEED * magnitude);
	float z = CAM_SHAKE_Z * znoise.noise1D(Time::elapsed * SHAKE_SPEED * magnitude);
	return glm::rotate(
		glm::rotate(
			glm::rotate(
				glm::mat4(1.0f), z, glm::vec3(0, 0, 1)
			), x, glm::vec3(0, 1, 0)
		), y, glm::vec3(0, 0, 1)
	);
}

void ChaseCam::cam_Update()
{	
	trauma = std::clamp(trauma, 0.0f, 1.0f);
	if (trauma > 0.0f) trauma -= Time::dt;

	if (!Input::getKey(GLFW_KEY_C)) 
		rot += Time::usdt * 2.0f * (cursor_rot - rot);
	
	glm::vec3 viewForward;
	viewForward.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
	viewForward.y = sin(glm::radians(rot.y));
	viewForward.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));
	
	viewForward = glm::normalize(viewForward);

	glm::vec3 camPos = *target - (viewForward * distance);

	view = getCamShake() * glm::lookAt(camPos, *target, glm::vec3(0, 1, 0));
	proj = glm::perspective(glm::radians(fov), window->aspect, 0.1f, 3000.0f);

	glm::vec2 cursorOffset = (cursor_rot - rot) * glm::vec2(0.01f, -0.015f);

	float ox = 0.025;
	float oy = Window::active->aspect * ox;
	
	cursor->bottom = cursorOffset.y - oy;
	cursor->top    = cursorOffset.y + oy;
	cursor->left   = cursorOffset.x - ox;
	cursor->right  = cursorOffset.x + ox;

	cursorForward.x = cos(glm::radians(cursor_rot.x)) * cos(glm::radians(cursor_rot.y));
	cursorForward.y = sin(glm::radians(cursor_rot.y));
	cursorForward.z = sin(glm::radians(cursor_rot.x)) * cos(glm::radians(cursor_rot.y));
	
	mouseIn(Input::Mouse::delta_x, Input::Mouse::delta_y);
}

ChaseCam::ChaseCam(float fov, float distance, Window* win, Hud* hud):
	target(target), trauma(0.0f), fov(fov), distance(distance)
{
	window = win;
	cursor = hud->AddTexture(0, 0, 0, 0, "res/textures/cursor.png");
	cursor_rot = glm::vec2(90.0f, 0);
	rot = cursor_rot;
	proj = glm::perspective(glm::radians(fov), win->aspect, 0.1f, 1000.0f);
	xnoise = siv::PerlinNoise(0);
	ynoise = siv::PerlinNoise(1);
	znoise = siv::PerlinNoise(2);
}

void ChaseCam::setTarget(glm::vec3* target)
{
	this->target = target;
}

void ChaseCam::mouseIn(double dx, double dy) {
	if (Input::getKey(GLFW_KEY_C)) {
		rot.x += dx * SENS_X;
		rot.y += dy * SENS_Y;
	}
	else {
		cursor_rot.x += dx * SENS_X;
		cursor_rot.y += dy * SENS_Y;
		cursor_rot.y = std::clamp(cursor_rot.y, -89.0f, 89.0f);
	}
}

void ChaseCam::addTrauma(int toADD)
{
	switch (toADD) {
		case 0: trauma += 0.3f; break;
		case 1: trauma += 0.5f; break;
		case 2: trauma += 0.7f; break;
	}
}

void ChaseCam::setTrauma(float toSet)
{
	if(trauma < toSet) trauma = toSet;
}
