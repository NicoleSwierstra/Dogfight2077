#include "Ground.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <thread>

Ground::Ground(int divs, int width, int height, glm::mat4* view, glm::mat4* proj, btDynamicsWorld* PhysicsWorld)
	: width(width), height(height), divs(divs), proj(proj), view(view), perlin(time(NULL))
{
	ground_shad = new Shader("res/shaders/ground.shader");
	line_shad = new Shader("res/shaders/lines.shader");

	int _w = (width * 2) / divs + 1;
	int _h = (height * 2) / divs + 1;

	std::vector<unsigned int> indexes;
	std::vector<glm::vec3> verts;

	bool flip = false;
	for (int y = -height; y <= height; y += divs) {
		for (int x = -width; x <= width; x += divs) {
			float h = perlin.accumulatedOctaveNoise2D_0_1((double)(x - (flip ? divs * 0.5f : 0)) * 0.02, (double)y * 0.02, 8) * MAX_HEIGHT;
			verts.push_back(glm::vec3(x - (flip ? divs * 0.5f : 0), h, y));
			if (y != height && x != width) {
				//have fun figuring this out in two months!
				unsigned int cx = (x + width) / divs, cy = (y + height) / divs; //current x/current y
				unsigned int cvert = (cy * _w) + cx, cvertp1 = ((cy + 1) * _w) + cx; // current vertex, current vertex on next row
				if (flip)
					indexes.insert(indexes.end(),
						{
							cvert, cvertp1, cvert + 1,
							cvertp1, cvertp1 + 1, cvert + 1
						}
				);
				else
					indexes.insert(indexes.end(),
						{
							cvert, cvertp1, cvertp1 + 1,
							cvert, cvertp1 + 1, cvert + 1
						}
				);
			}
		}
		flip = !flip;
	}
	m = new Mesh(verts, indexes);
	m->AddShader(
		ground_shad, proj, view,
		{ uniformNode{"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, false
	)->AddShader(
		line_shad, proj, view,
		{ uniformNode{"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, true
	);
	m->Optimise();
	pos = glm::vec3(0, -30, 0);
	updateTrans();

	btTriangleMesh* mesh = new btTriangleMesh();

	for (int i = 0; i < verts.size(); i++) {
		mesh->findOrAddVertex(toBT(verts[i]), false);
	}
	for (int i = 0; i < indexes.size(); i+=3) {
		mesh->addTriangleIndices(indexes[i], indexes[i+1], indexes[i+2]);
	}

	btCollisionShape* worldShape =
		new btBvhTriangleMeshShape(mesh, true);
	
	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(toBT(pos));
	
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	
	btScalar mass(0.0);
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
		motionState,
		worldShape,
		localInertia);
	btRigidBody* rb = new btRigidBody(rbInfo);
	rb->setUserIndex(-2);
	rb->setUserPointer(this);
	PhysicsWorld->addRigidBody(rb);
}

Ground::~Ground()
{
	delete m;
	delete ground_shad;
	delete line_shad;
}

void Ground::Update() {

}

void Ground::Render()
{
	m->Render();
}