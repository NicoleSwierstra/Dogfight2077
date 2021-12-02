#include "Targets.h"
#include "PerlinNoise.hpp"
#include <time.h>
#include <cmath>
#include <iostream>

Targets* Targets::tg = new Targets();

void Targets::init(btDiscreteDynamicsWorld* world, glm::mat4* view, glm::mat4* proj)
{
	this->world = world;
	targetMesh = new Mesh("res/models/cube.obj");
	targetMesh->AddShader(new Shader("res/shaders/target.shader"), proj, view, {}, 0, GL_TRIANGLES, false);

	btBoxShape* shape = new btBoxShape(btVector3(2.0f, 2.0f, 2.0f));

	healths = std::vector<float>(50, 1.0f);

	siv::PerlinNoise perlin(time(NULL));
	for (int i = 0; i < healths.size(); i++) {
		float x = -2048.0f + (perlin.noise1D_0_1(i * 123.32189) * 4096.0f),
			  y = (perlin.noise1D_0_1(i * 59123.32189) * 128.0f) + 32.0f,
			  z = -2048.0f + (perlin.noise1D_0_1(i * 353.32189) * 4096.0f);

		std::cout << x << "," << y << "," << z << "\n";

		btTransform transform = btTransform();
		transform.setIdentity();
		transform.setOrigin(btVector3(x, y, z));

		btDefaultMotionState* motionState = new btDefaultMotionState(transform);

		btScalar mass(0.0);
		btVector3 localInertia(0, 0, 0);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
			motionState,
			shape,
			localInertia);
		btRigidBody* rb = new btRigidBody(rbInfo);
		rb->setUserIndex(i);

		world->addRigidBody(rb);

		targetTrans.push_back(glm::vec3(x, y, z));
		targetRB.push_back(rb);
		un.push_back(uniformNode{ "health", &healths[i], UNIFORM_FLOAT});
	}
}

void Targets::remove(int index)
{
	if (index < 0) return;
	healths[index] -= 0.1f;
	if (healths[index] > 0) return;

	btRigidBody* rb = targetRB[index];
	targetRB.erase(targetRB.begin() + index);
	targetTrans.erase(targetTrans.begin() + index);
	healths.erase(healths.begin() + index);
	un.erase(un.begin() + index);
	world->removeRigidBody(rb);

	//making sure to reset the user pointers on all of those rigidbodies
	for (int i = index; i < targetRB.size(); i++) {
		targetRB[i]->setUserIndex(i);
		un[i].data = &healths[i];
	}

	delete rb;
}

void Targets::render() {
	targetMesh->RenderMultiple(targetTrans, un, "trans");
}