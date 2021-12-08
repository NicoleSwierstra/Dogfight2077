#include "Player.h"
#include "util/Time.h"
#include "Input/Handler.h"'
#include <algorithm>
#include "Physics/Bullet.h"
#include "graphical/BulletHandler.h"
#include "Targets.h"
#include "bullet/btBulletCollisionCommon.h"
#include "Enemy.h"
#include "oop/SceneHandler.h"

struct groundcol : btCollisionWorld::ContactResultCallback {
	bool withGround = false;

	btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap,
		int 	partId0,
		int 	index0,
		const btCollisionObjectWrapper* colObj1Wrap,
		int 	partId1,
		int 	index1
	) override {
		if (colObj0Wrap->getCollisionObject()->getUserIndex() == -2 ||
			colObj1Wrap->getCollisionObject()->getUserIndex() == -2) withGround = true;
		return 1;
	}
};

//I hope this isn't broken
glm::quat normalize(glm::quat& quat) {
	float m =
		quat.x * quat.x +
		quat.y * quat.y +
		quat.z * quat.z +
		quat.w * quat.w;
	__m128 l = _mm_load_ss(&m);
	float length;
	_mm_store_ss(&length, _mm_rsqrt_ss(l));
	if (length == 1.0f || m == 0) return quat;
	return glm::quat(quat * length);
};

Player::Player(ChaseCam* cam, btDiscreteDynamicsWorld* world) : cam(cam), PhysicsWorld(world)
{
	crosshair = SceneHandler::active->currentHud->AddTexture(0, 0, 0, 0, "res/textures/cursor.png");
	name = "player";
    mesh = new Mesh("res/models/fighter.obj");
	lines = new Shader("res/shaders/planelines.shader");
	body = new Shader("res/shaders/player.shader");
	cockpit = new Shader("res/shaders/cockpit.shader");
    mesh->AddShader(body, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, false)
		->AddShader(lines, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4} }, 1, GL_TRIANGLES, false)
		->AddShader(cockpit, &cam->proj, &cam->view, { {"trans", &trans, UNIFORM_MAT4}, {"health", &health, UNIFORM_FLOAT}}, 2, GL_TRIANGLES, false);
	mesh->Optimise();

	rot = glm::quat(glm::vec3(0, 0, 0));
	pos = glm::vec3(0.0f, 60.0f, -100.0f);

	btCollisionShape* planeShape =
		new btBoxShape(btVector3(btScalar(3.), btScalar(1.), btScalar(4.)));

	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(toBT(pos));
	transform.setRotation(btQuaternion(0.0, 0.0, 0.0));
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
	rb->setUserIndex(-4);

	world->addRigidBody(rb);
}

void Player::shoot()
{
	static float timer = 0.0f;
	static bool left;
	static siv::PerlinNoise p1(time(NULL));
	static siv::PerlinNoise p2(time(NULL) + 1);
	timer -= Time::dt;
	if (timer < 0.0f) {
		timer += 0.04f;
		glm::vec3 barrelOffset0 = Physics.front * 0.8f + Physics.right * 0.6f;
		glm::vec3 barrelOffset1 = Physics.front * 0.8f - Physics.right * 0.6f;
		glm::vec3 end = glm::normalize(Physics.front + ((glm::vec3(p1.noise1D(Time::elapsed * 13.5f), p2.noise1D(Time::elapsed * 13.7f), 0) * rot) * SHOOTSPREAD));

		bulletHandler::addBullet(
			pos + (left ? barrelOffset0 : barrelOffset1),
			end
		);
		left = !left;

		glm::vec3 source = pos + (end * 2.0f);
		glm::vec3 endpnt = pos + (end * 500.0f);

		btCollisionWorld::ClosestRayResultCallback rr(toBT(source), toBT(endpnt));

		PhysicsWorld->rayTest(toBT(source), toBT(endpnt), rr);

		if (rr.hasHit()) {
			if (rr.m_collisionObject->getUserIndex() == -3) {
				if(rr.m_collisionObject->getUserPointer() != nullptr)
					((Enemy*)rr.m_collisionObject->getUserPointer())->TakeDamage(0.1f);
			}
			else if(rr.m_collisionObject->getUserIndex() != -2)
				Targets::Remove(rr.m_collisionObject->getUserIndex());
		}
	}
}

glm::vec3* Player::getPos() {
	return &cam_target;
}

void Player::rotAngles(bool multi, float pitch, float roll, float yaw) {
	glm::vec3 rs = rot * glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
	
	if (multi) {
		glm::quat fr = glm::quat((rs * 0.1f) + glm::eulerAngles(rot));
		if (glm::length(fr) == 1.0f) {
			rb->getWorldTransform().setRotation(toBT(fr));
			rot = fr;
		}
		else {
			fr = glm::normalize(fr);
			rb->getWorldTransform().setRotation(toBT(fr));
			rot = fr;
		}
	}
	else {
		rb->setAngularVelocity(toBT(rs));
		btQuaternion q = rb->getCenterOfMassTransform().getRotation();
		rot = toGLM(q);
	}

	Physics.front = rot * glm::vec3(0, 0, 1);
	Physics.right = rot * glm::vec3(1, 0, 0);
	Physics.up    = rot * glm::vec3(0, 1, 0);

	glm::vec2 crosshair_rot(
		atan2(Physics.front.z, Physics.front.x) * 57.2957795131, asin(Physics.front.y) * 57.2957795131
	);

	glm::vec2 cam_rot = ((ChaseCam*)SceneHandler::active->camera)->rot;
	glm::vec2 full_rot = glm::mod(glm::abs(crosshair_rot - cam_rot), glm::vec2(180.0f, 180.0f)) * glm::sign(crosshair_rot - cam_rot);
	glm::vec2 cursorOffset = full_rot * glm::vec2(0.01f, -0.015f);

	//std::cout << full_rot.x << "," << full_rot.y << "\n";

	float ox = 0.01f;
	float oy = Window::active->aspect * ox;

	crosshair->bottom = cursorOffset.y - oy;
	crosshair->top = cursorOffset.y + oy;
	crosshair->left = cursorOffset.x - ox;
	crosshair->right = cursorOffset.x + ox;
}

bool Player::checkCOL() {
	groundcol c;
	PhysicsWorld->contactTest(rb, c);
	if (c.withGround)
		takeDamage(0.5f * Time::dt);
	return true;
}

void Player::Update()
{
	speed = dot(Physics.front, Physics.velocity);
	float downspeed = -dot(Physics.up, Physics.velocity);
	float latspeed = dot(Physics.right, Physics.velocity);

	throttle = std::clamp(throttle, 0.0f, 1.0f);

	if (Input::getKey(GLFW_KEY_SPACE) || Input::getMouseButton(GLFW_MOUSE_BUTTON_1)) {
		shoot();
	}

	
	float speedExp = log2(abs(speed + 1));

	bool mouse2 = Input::getMouseButton(1);

	if (mouse2 && canBoost) {
		slowed = true;
		canBoost = false;
		Time::scale = 0.25f;
	}
	else if (slowed && mouse2) {
		if (boostTimer > BOOSTTIMER) {
			boostTimer = 0.0f;
			slowed = false;
		}
		boostTimer += Time::dt;
	}
	else {
		if (boostTimer > 0.0f) {
			boostTimer -= Time::dt;
			cam->setTrauma(0.2f);
			boosting = true;
		}
		else {
			boosting = false;
			canBoost = true;
		}
		slowed = false;
		Time::scale = 1.0f;
	}
	
	//angles
	float rAuthority = ROLL_AUTHORITY * speedExp,
		pAuthority = PITCH_AUTHORITY  * speedExp,
		yAuthority = YAW_AUTHORITY    * speedExp;

	float deltaroll = 0;
	float deltapitch = 0;
	float deltayaw = 0;

	float inroll = Input::getAxis(GLFW_KEY_E, GLFW_KEY_Q);
	float inpitch = Input::getAxis(GLFW_KEY_W, GLFW_KEY_S);
	float inyaw = Input::getAxis(GLFW_KEY_A, GLFW_KEY_D);

	if (inpitch != 0 || inyaw != 0 || inroll != 0) {
		deltaroll  =  inroll  * rAuthority;
		deltapitch = (inpitch + (downspeed * 0.05f)) * pAuthority;
		deltayaw   = (inyaw   + (latspeed * 0.05f)) * yAuthority;
	}
	else {
		const glm::vec3& target = cam->cursorForward;
		glm::vec3 localFlyTarget = target * rot * 25.0f;
		float angleOffTarget = glm::acos(glm::dot(Physics.front, glm::normalize(target)));

		deltayaw = std::clamp(localFlyTarget.x, -1.f, 1.f) * (slowed ? PITCH_AUTHORITY : YAW_AUTHORITY);
		deltapitch = -std::clamp(localFlyTarget.y, -1.f, 1.f) * PITCH_AUTHORITY;

		float agressiveRoll = -std::clamp(localFlyTarget.x * 0.5f, -1.f, 1.f);
		float wingsLevelRoll = -Physics.right.y;
		float wingsLevelInfluence = angleOffTarget / glm::radians(30.0f);

		deltaroll = glm::mix(wingsLevelRoll, agressiveRoll, slowed ? 0.1f : wingsLevelInfluence) * ROLL_AUTHORITY;
	}

	//rot angles
	rotAngles(slowed, deltapitch, deltaroll, deltayaw);
	
	//forces
	glm::vec3 lift = Physics.up * LIFT * (speed + downspeed) * glm::sign(glm::dot(Physics.up, glm::vec3(0, 1, 0)));
	glm::vec3 thrust = Physics.front * THRUST * (throttle + (boosting ? BOOSTSPEED : 0.0f));
	glm::vec3 drag = (-Physics.front * (speed * speed * DRAG)) + 
		(Physics.right * -latspeed) +
		(Physics.up * downspeed);

	Physics.velocity += (lift + thrust + drag + GRAVITY) * Time::dt;

	rb->setLinearVelocity(toBT(Physics.velocity));
	btVector3 bpos = rb->getCenterOfMassTransform().getOrigin();
	pos = toGLM(bpos);

	cam_target = pos + (Physics.up * 0.75f);

	checkCOL();
}

void Player::takeDamage(float dmg) {
	health -= dmg;
	cam->addTrauma(0);
}

void Player::Render()
{
	updateTrans();

	lines->Bind();
	lines->SetFloatUniforms("timer", boostTimer);

	mesh->Render();
}
