#pragma once
#include "Scene.h"

class SceneHandler {
	static void internalChange();
public:
	static Scene* active, *queued;
	TextRenderer* tr;

	static void changeScene(Scene* nextscene);

	static void Update() { if(active) active->Update(); internalChange(); }
	static void Render() { if(active) active->Render(); }
};