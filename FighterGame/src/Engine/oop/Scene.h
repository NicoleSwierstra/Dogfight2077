#pragma once
#include <vector>
#include "gui/Hud.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "Cameras/Camera.h"
#include "GameObject.h"
#include <iostream>

class Scene {
public:
	std::vector<GameObject*> objects;
	std::vector<int> toDelete;
	Camera* camera;
	Hud* currentHud;

	template <class g>
	g addGameObject(g object) {
		objects.push_back((GameObject*)object);
		((GameObject*)object)->scene = this;
		return object;
	}

	virtual void init() = 0;

	void Update() {
		for (GameObject* o : objects) {
			o->Update();
		}
		camera->cam_Update();

		for (int index : toDelete) {
			objects[index]->scene = nullptr;
			delete objects[index];
			objects.erase(objects.begin() + index);
			std::cout << "DeleteIndex: " << index << "\n";
		}
		toDelete.clear();
	}

	void Render() {
		for (GameObject* o : objects) {
			o->Render();
		}
		currentHud->Render();
	}

	void clearScene() {
		for (GameObject* o : objects) {
			delete o;
		}
		objects.clear();
	}

	void deleteGO(GameObject* go) {
		try {
			for (int i = 0; i < objects.size(); i++) {
				if (go == objects[i]) {
					toDelete.push_back(i);
				}
			}
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
	}
};