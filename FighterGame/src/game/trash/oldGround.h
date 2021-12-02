#pragma once
#include "graphics/Mesh.h"
#include "oop/GameObject.h"
#include "PerlinNoise.hpp"
#include <map>
#include <memory>

namespace std {
	template <>
	struct hash<glm::ivec2>
	{
		std::size_t operator()(const glm::ivec2& k) const
		{
			using std::hash;
			size_t val = *(size_t*)(void*)&k;
			return hash<size_t>()(val);
		}
	};
}

class Ground : GameObject {
	float MAX_HEIGHT = 30.0f;
	Shader* ground_shad;
	Shader* line_shad;

	//rendering
	glm::mat4 * view, * proj;
	
	//logic
	glm::vec3* campos;
	glm::ivec2 currentChunk;

	//generation
	const siv::PerlinNoise perlin;
	const uint32_t width, height, divs;
	
	//good question
	std::unordered_map<glm::ivec2, std::unique_ptr<Mesh>> meshes;
	std::unordered_map<glm::ivec2, bool> generating_flag;
	void createMesh(int xoffset, int yoffset);
	std::vector<glm::vec2> getAdj();
public:
	//DIVS NEEDS TO BE A MULTIPLE OF THE WIDTH OR THE HEIGHT OR THIS ALGORITHM WILL BREAK DRAMATICALLY
	Ground(int divs, int width, int height, glm::mat4* view, glm::mat4* proj, glm::vec3* campos);

	void Update() override;
	void Render() override;
};