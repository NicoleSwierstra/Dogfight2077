#pragma once

#include "oop/GameObject.h"
#include "glm/glm.hpp"
#include "graphics/Mesh.h"
#include "Physics/Bullet.h"
#include "Cameras/ChaseCam.h"

class Enemy : GameObject {
	const float PITCH_AUTHORITY = 10.5f;
	const float YAW_AUTHORITY = 5.5f;
	const float ROLL_AUTHORITY = 15.0f;

	const float THRUST = 30.0f;
	const float LIFT = 0.3f;
	const float DRAG = 0.04f;
	const float STABILITY = 0.005f;

	const float SHOOTSPREAD = .0125f;
	const float SHOOTRANGE = 100.0f;

	const glm::vec3 GRAVITY = glm::vec3(0, -9.81f, 0);

	struct {
		glm::vec3 velocity = glm::vec3(0, 0, 50.0f),
			front = glm::vec3(0, 0, 1),
			right = glm::vec3(1, 0, 0),
			up = glm::vec3(0, 1, 0);
	} Physics;

	Shader* lines, * body, * cockpit;
	Mesh* mesh;
	btRigidBody* rb;
	
	float shoottimer = 0.0f;
	bool shootleft;
	float health = 1.0f;
public:
	float speed;
	static GameObject* player;
	btDiscreteDynamicsWorld* PhysicsWorld;

	Enemy(ChaseCam* cam, btDiscreteDynamicsWorld* world, glm::vec3 position = glm::vec3(0.0f, 30.0f, 40.0f), glm::vec3 rotation = glm::vec3(0.0f));
	~Enemy();

	void rotAngles(float pitch, float roll, float yaw);
	void shoot();
	void TakeDamage(float dmg);

	void Update() override;
	void Render() override;
};