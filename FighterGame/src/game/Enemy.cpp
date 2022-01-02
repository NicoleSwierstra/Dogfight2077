#include "Enemy.h"
#include "util/Time.h"
#include "graphical/BulletHandler.h"
#include "Player.h"
#include "scenes/CombatScene.h"

GameObject* Enemy::player;

Enemy::Enemy(ChaseCam* cam, btDiscreteDynamicsWorld* world, glm::vec3 position, glm::vec3 rotation) : PhysicsWorld(world)
{
	name = "enemy";
	mesh = new Mesh("res/models/fighter.obj");
	lines = new Shader("res/shaders/planelines.shader");
	body = new Shader("res/shaders/player.shader");
	cockpit = new Shader("res/shaders/cockpit.shader");
	mesh->AddShader(body, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, false)
		->AddShader(lines, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4} }, 1, GL_LINES, false)
		->AddShader(cockpit, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4}, {"health", &health, UNIFORM_FLOAT} }, 2, GL_TRIANGLES, false);
	mesh->Optimise();

	rot = glm::quat(rotation);
	pos = glm::vec3(position);

	btCollisionShape* planeShape =
		new btBoxShape(btVector3(btScalar(3.), btScalar(1.), btScalar(4.)));

	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(toBT(pos));
	transform.setRotation(toBT(rot));
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	btScalar mass(5.0);
	btVector3 localInertia;

	planeShape->calculateLocalInertia(mass, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
		motionState,
		planeShape,
		localInertia);
	rb = new btRigidBody(rbInfo);
	rb->setUserPointer(this);
	rb->setUserIndex(-3);

	world->addRigidBody(rb);
}

Enemy::~Enemy()
{
	rb->setUserPointer(nullptr);
	PhysicsWorld->removeRigidBody(rb);
	delete lines;
	delete cockpit;
	delete body; 
	delete mesh; 
	delete rb;
}

void Enemy::rotAngles(float pitch, float roll, float yaw)
{
	glm::vec3 rs = rot * glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));

	rb->setAngularVelocity(toBT(rs));
	btQuaternion q = rb->getCenterOfMassTransform().getRotation();
	rot = toGLM(q);

	Physics.front = rot * glm::vec3(0, 0, 1);
	Physics.right = rot * glm::vec3(1, 0, 0);
	Physics.up = rot * glm::vec3(0, 1, 0);
}

void Enemy::shoot()
{
	static siv::PerlinNoise p1(time(NULL));
	static siv::PerlinNoise p2(time(NULL) + 1);
	shoottimer -= Time::dt;
	if (shoottimer < 0.0f) {
		shoottimer += 0.25f;
		glm::vec3 barrelOffset0 = Physics.front * 0.8f + Physics.right * 0.6f;
		glm::vec3 barrelOffset1 = Physics.front * 0.8f - Physics.right * 0.6f;
		glm::vec3 end = glm::normalize(Physics.front + ((glm::vec3(p1.noise1D(Time::elapsed * 13.5f), p2.noise1D(Time::elapsed * 13.7f), 0) * rot) * SHOOTSPREAD));

		bulletHandler::addBullet(
			pos + (shootleft ? barrelOffset0 : barrelOffset1),
			end
		);
		shootleft = !shootleft;

		glm::vec3 source = pos + (end * 2.0f);
		glm::vec3 endpnt = pos + (end * 500.0f);

		btCollisionWorld::ClosestRayResultCallback rr(toBT(source), toBT(endpnt));

		PhysicsWorld->rayTest(toBT(source), toBT(endpnt), rr);

		if (rr.hasHit()) {
			if (rr.m_collisionObject->getUserIndex() == -4)
				((Player*)player)->takeDamage(0.025f);
		}
	}
}

void Enemy::TakeDamage(float dmg)
{
	if (health > 0) health -= dmg;
	else if(scene && scene != (void*)0xdddddddddddddddd) {
		scene->deleteGO(this);
	}
}

void Enemy::Update()
{
	if(pos.y < 10.0f) scene->deleteGO(this);

	speed = dot(Physics.front, Physics.velocity);
	float downspeed = -dot(Physics.up, Physics.velocity);
	float latspeed = dot(Physics.right, Physics.velocity);

	float speedExp = 3.0f;

	//angles
	float rAuthority = ROLL_AUTHORITY * speedExp,
		pAuthority = PITCH_AUTHORITY * speedExp,
		yAuthority = YAW_AUTHORITY * speedExp;

	float dist = glm::length(player->pos - pos);
	glm::vec3 target = glm::normalize(player->pos - pos);
	glm::vec3 localFlyTarget = target * rot * 25.0f;
	float angleOffTarget = glm::acos(glm::dot(Physics.front, glm::normalize(target)));
	
	float deltayaw = std::clamp(localFlyTarget.x, -1.f, 1.f) * YAW_AUTHORITY;
	float deltapitch = -std::clamp(localFlyTarget.y, -1.f, 1.f) * PITCH_AUTHORITY;
	
	float agressiveRoll = -std::clamp(localFlyTarget.x * 0.5f, -1.f, 1.f);
	float wingsLevelRoll = -Physics.right.y;
	float wingsLevelInfluence = angleOffTarget / glm::radians(30.0f);
	
	float deltaroll = glm::mix(wingsLevelRoll, agressiveRoll, wingsLevelInfluence) * ROLL_AUTHORITY;
	
	if (angleOffTarget < glm::radians(30.0f) && dist < SHOOTRANGE) {
		shoot();
	}

	//rot angles
	rotAngles(deltapitch, deltaroll, deltayaw);

	glm::vec3 thrust = Physics.front * THRUST;

	Physics.velocity = thrust;

	rb->setLinearVelocity(toBT(Physics.velocity));
	btVector3 bpos = rb->getCenterOfMassTransform().getOrigin();
	pos = toGLM(bpos);
}

void Enemy::Render()
{
	updateTrans();
	mesh->Render();
}
