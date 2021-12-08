#pragma once
#include "oop/Scene.h"

class CombatScene : public Scene {
	std::string enemiesstring;
	int getenemies();
public:
	CombatScene(Hud* h);
	void init() override;
	void Update() override;
};