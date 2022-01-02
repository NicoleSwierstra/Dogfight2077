#pragma once
#include "oop/Scene.h"
#include "Player.h"

class CombatScene : public Scene {
	Player* p;
	std::string enemiesstring;
	int getenemies();
public:
	CombatScene(Hud* h);
	void init() override;
	void Update() override;
};