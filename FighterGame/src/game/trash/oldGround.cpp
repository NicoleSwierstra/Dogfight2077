#include "Ground.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <thread>
#pragma once

Ground::Ground(int divs, int width, int height, glm::mat4* view, glm::mat4* proj, glm::vec3* campos)
	: width(width), height(height), divs(divs), perlin(time(NULL))
{
	this->campos = campos;
	this->view = view;
	this->proj = proj;
	ground_shad = new Shader("res/shaders/ground.shader");
	line_shad = new Shader("res/shaders/lines.shader");
	pos = glm::vec3(0, -30, 0);
	currentChunk = glm::ivec2(0,0);
	updateTrans();
}

void Ground::createMesh(int xchunk, int ychunk) {
	if (meshes.find(glm::ivec2(xchunk, ychunk)) != meshes.end()) return;
	generating_flag[glm::ivec2(xchunk, ychunk)] = true;

	std::cout << "THREADS WORK!\n";
	
	std::vector<unsigned int> indexes;
	std::vector<glm::vec3> verts;

	//the true width and height of the for loop
	int _w = (width * 2) / divs + 1;
	int _h = (height * 2) / divs + 1;
	int yoffset = ychunk * height * 2;
	int xoffset = xchunk * width * 2;

	bool flip = false;
	for (int y = yoffset-height; y <= height + yoffset; y += divs) {
		for (int x = xoffset-width; x <= width + xoffset; x += divs) {
			float h = perlin.accumulatedOctaveNoise2D_0_1((double)(x - (flip ? divs * 0.5f : 0)) * 0.02, (double)y * 0.02, 8) * MAX_HEIGHT;
			verts.push_back(glm::vec3(x - (flip ? divs * 0.5f : 0), h, y));
			if (y != height && x != width) {
				//have fun figuring this out in two months!
				unsigned int cx = (x + width - xoffset) / divs, cy = (y + height - yoffset) / divs; //current x/current y
				unsigned int cvert = (cy * _w) + cx, cvertp1 = ((cy + 1) * _w) + cx; // current vertex, current vertex on next row
				//std::cout << "CHUNK:(" << xchunk << ", " << ychunk << ") xoff: " 
				//	<< xoffset << ", yoff:" << yoffset << ", cverts:" << cvert << "," << cvertp1 << ", current:(" << cx << "," << cy << "), " 
				//	<< cvertp1 + 1 << "," << verts.size() << "\n" << std::flush;
				//makes triangles into approximately equaliteral tris for hex like appearence
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

	try {
		Mesh* m = new Mesh(verts, indexes);
		m->AddShader(
			ground_shad, proj, view,
			{ uniformNode{"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, false
		)->AddShader(
			line_shad, proj, view,
			{ uniformNode{"trans", &trans, UNIFORM_MAT4} }, 0, GL_TRIANGLES, true
		);
		meshes[glm::ivec2(xchunk, ychunk)] = std::unique_ptr<Mesh>(m);
	}
	catch (int e) {
		std::cout << "SIZE OF VERTS IS ZERO @ ! " << xchunk << ", " << ychunk << "\n";
	}

	generating_flag[glm::ivec2(xchunk, ychunk)] = false;
}


std::vector<glm::vec2> Ground::getAdj() 
{
	std::vector<glm::vec2> adjm;
	adjm.push_back(currentChunk + glm::ivec2(-1, -1));
	adjm.push_back(currentChunk + glm::ivec2( 0, -1));
	adjm.push_back(currentChunk + glm::ivec2( 1, -1));
	adjm.push_back(currentChunk + glm::ivec2(-1,  0));
	adjm.push_back(currentChunk);
	adjm.push_back(currentChunk + glm::ivec2( 1,  0));
	adjm.push_back(currentChunk + glm::ivec2(-1,  1));
	adjm.push_back(currentChunk + glm::ivec2( 0,  1));
	adjm.push_back(currentChunk + glm::ivec2( 1,  1));
	return adjm;
}

void Ground::Update()
{
	currentChunk = glm::ivec2(campos->x / width * 2, campos->y / height * 2);
}

void Ground::Render()
{
	for (glm::ivec2 v : getAdj()) {
		if (meshes.find(v) != meshes.end()) {
			std::cout << "RENDERING!\n";
			meshes[v]->Render();
		}
		else if (!generating_flag[v]) {
			std::cout << "THREAD " << v.x << "," << v.y << "\n";
			new std::thread([this, v] { createMesh(v.x, v.y); });
		}
	}
}