#pragma once
#include "oop/GameObject.h"
#include "glm/glm.hpp"
#include "graphics/Mesh.h"
#include "Physics/Bullet.h"

class Player : GameObject {
	const float PITCH_AUTHORITY = 8.5f;
	const float YAW_AUTHORITY = 5.5f;
	const float ROLL_AUTHORITY = 12.0f;

	const float THRUST = 40.0f;
	const float LIFT = 0.4f;
	const float DRAG_F = 0.04f;
	const float DRAG_R = 0.2f;
	const float DRAG_U = 0.4f;
	const float STABILITY = 0.005f;

	btRigidBody * rb;

	Mesh * mesh;

public:
	Player(glm::mat4* proj, glm::mat4* view, btDynamicsWorld* PhysicsWorld);

	glm::vec3* getPos();

	void Update() override;
	void Render() override;
};