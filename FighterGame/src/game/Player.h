#pragma once
#include "oop/GameObject.h"
#include "glm/glm.hpp"
#include "graphics/Mesh.h"
#include "Physics/Bullet.h"
#include "Cameras/ChaseCam.h"

class Player : public GameObject {
	const float PITCH_AUTHORITY = 15.5f;
	const float YAW_AUTHORITY = 5.5f;
	const float ROLL_AUTHORITY = 20.0f;

	const float THRUST = 40.0f;
	const float LIFT = 0.3f;
	const float DRAG = 0.04f;
	const float STABILITY = 0.005f;

	const float BOOSTSPEED  = 3.0f;
	const float BOOSTTIMER  = 2.0f;

	const float SHOOTSPREAD = .0125f;

	const glm::vec3 GRAVITY = glm::vec3(0, -9.81f, 0);

	struct {
		glm::vec3 velocity = glm::vec3(0, 0, 50.0f), 
			front  = glm::vec3(0, 0, 1), 
			right  = glm::vec3(1, 0, 0), 
			up     = glm::vec3(0, 1, 0);
	} Physics;

	Shader* lines, * body, * cockpit;
	Mesh * mesh;
	ChaseCam* cam;
	btRigidBody* rb;

	float boostTimer = 0.0f;
	bool boosting = false, slowed = false, canBoost = true;

	glm::vec3 cam_target;
	texInstance* crosshair;

	float health = 1.0f;
public:
	float throttle = 1.0f, speed = 0.0f;
	btDiscreteDynamicsWorld* PhysicsWorld;

	Player(ChaseCam* cam, btDiscreteDynamicsWorld* world);

	void rotAngles(bool multi, float pitch, float roll, float yaw);
	void shoot();
	void takeDamage(float dmg);
	bool checkCOL();

	glm::vec3* getPos();

	void Update() override;
	void Render() override;
};