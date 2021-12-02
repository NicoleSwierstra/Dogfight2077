#pragma once
#include "Physics/Bullet.h"
#include <vector>
#include "graphics/Mesh.h"

class Targets {
	static Targets* tg;

	Mesh* targetMesh;

	btDiscreteDynamicsWorld* world;
	std::vector<glm::vec3> targetTrans;
	std::vector<btRigidBody*> targetRB;
	
	std::vector<uniformNode> un;
	std::vector<float> healths;

	void init(btDiscreteDynamicsWorld* world, glm::mat4* view, glm::mat4* proj);
	void remove(int index);
	void render();
public:
	static void Init(btDiscreteDynamicsWorld* world, glm::mat4* view, glm::mat4* proj) { tg->init(world, view, proj); }
	static void Remove(int index) { tg->remove(index); }
	static void Render() { tg->render(); }
};