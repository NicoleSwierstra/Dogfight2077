#pragma once
#include "graphics/Mesh.h"
#include "oop/GameObject.h"
#include "PerlinNoise.hpp"

#include "Physics/Bullet.h"

class Ground : GameObject {
	float MAX_HEIGHT = 30.0f;
	Shader* ground_shad;
	Shader* line_shad;
	Mesh* m;

	//rendering
	glm::mat4 * view, * proj;

	//data
	const siv::PerlinNoise perlin;
	const uint32_t width, height, divs;
public:
	//DIVS NEEDS TO BE A MULTIPLE OF THE WIDTH OR THE HEIGHT OR THIS ALGORITHM WILL BREAK DRAMATICALLY
	Ground(int divs, int width, int height, glm::mat4* view, glm::mat4* proj, btDynamicsWorld* PhysicsWorld);
	
	~Ground();

	void Update() override;
	void Render() override;
};