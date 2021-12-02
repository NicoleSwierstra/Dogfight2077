#include "Player.h"
#include "util/Time.h"
#include "Input/Handler.h"'
#include <algorithm>

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

Player::Player(glm::mat4* proj, glm::mat4* view, btDynamicsWorld* PhysicsWorld)
{
    mesh = new Mesh("res/models/fighter.obj");
	Shader* lines = new Shader("res/shaders/lines.shader");
	Shader* body = new Shader("res/shaders/player.shader");
	Shader* cockpit = new Shader("res/shaders/cockpit.shader");
    mesh->AddShader(body, proj, view, { {"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, false)
		->AddShader(lines, proj, view, { {"trans", &trans, UNIFORM_MAT4} }, 1, GL_TRIANGLES, false)
		->AddShader(cockpit, proj, view, { {"trans", &trans, UNIFORM_MAT4} }, 2, GL_TRIANGLES, false);
	mesh->Optimise();

	rot = glm::quat(glm::vec3(0, 0, 0));

	btCollisionShape* planeShape = 
		new btBoxShape(btVector3(btScalar(3.), btScalar(4.), btScalar(1.)));

	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setRotation(btQuaternion(0.0, 0.0, 0.0));
	btDefaultMotionState *motionState = new btDefaultMotionState(transform);

	btScalar mass(5.0);
	
	btVector3 localInertia(0, 0, 0);

	planeShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, 
		motionState, 
		planeShape, 
		localInertia);
	rb = new btRigidBody(rbInfo);

	PhysicsWorld->addRigidBody(rb);
}

glm::vec3* Player::getPos() {
	return &pos;
}

void Player::Update()
{
	btTransform transform = rb->getCenterOfMassTransform();
	btVector3 velocity = rb->getLinearVelocity();

	glm::vec3 forward = glm::vec3(0, 0, 1.0) * rot;
	glm::vec3 right = glm::vec3(1.0, 0, 0) * rot;
	glm::vec3 up = glm::vec3(0, 1.0, 0) * rot;

	btVector3 drag_lin;
	btVector3 drag_rot;
	{
		glm::vec3 ldrag = glm::vec3(-DRAG_R, -DRAG_U, -DRAG_F) * rot;
		drag_lin = velocity * velocity.length() * toBT(ldrag);
	}

	rb->applyTorque(btVector3(
		Input::getAxis(GLFW_KEY_W, GLFW_KEY_S),
		Input::getAxis(GLFW_KEY_D, GLFW_KEY_A),
		Input::getAxis(GLFW_KEY_Q, GLFW_KEY_E)
	));

	rb->applyCentralForce(toBT(forward) * THRUST);
	rb->applyCentralForce(drag_lin);

	btVector3 bpos = transform.getOrigin();
	btQuaternion brot = transform.getRotation();
	pos = toGLM(bpos);
	rot = toGLM(brot);
}

void Player::Render()
{
	updateTrans();
	mesh->Render();
}
