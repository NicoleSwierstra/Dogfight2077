#pragma once
#include "oop/Scene.h"

class CombatScene : public Scene {
public:
	CombatScene(Hud* h);
	void init() override;
};