#include "CombatScene.h"
#include "Cameras/ChaseCam.h"
#include "Player.h"
#include "Physics/Bullet.h"
#include "Ground.h"
#include "Enemy.h"
#include <iostream>

CombatScene::CombatScene(Hud* hud)
{
    camera = new ChaseCam(70.0f, 8.0f, Window::active, hud);
    currentHud = hud;
}

void CombatScene::init() {
    Player* p = addGameObject(new Player((ChaseCam*)camera, Bullet.world));
    addGameObject(new Ground(16, 4096, 4096, &camera->view, &camera->proj, Bullet.world));
    for (int i = -5; i <= 5; i++) {
        addGameObject(new Enemy((ChaseCam*)camera, Bullet.world, glm::vec3(i * 5.0f, 40.0f, 120.0f)))->player = (GameObject*)p;
    }

    currentHud->AddText(&p->throttle, -0.95f, -0.95f, 0.0006f);
    currentHud->AddText(&p->speed, -0.95f, -0.875f, 0.0006f);

    ((ChaseCam*)camera)->setTarget(p->getPos());
}