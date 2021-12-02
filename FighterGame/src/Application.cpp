#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <optick.h>

#include "Engine/Window.h"
#include "Engine/Input/Handler.h"
#include "Engine/graphics/Texture.h"
#include "Engine/graphics/VertexBuffer.h"
#include "Engine/graphics/EBO.h"
#include "Engine/graphics/Shader.h"
#include "Engine/Cameras/ChaseCam.h"
#include "Engine/util/Time.h"
#include "Engine/graphics/Mesh.h"
#include "Engine/oop/GameObject.h"
#include "Engine/gui/Hud.h"
#include "Engine/Physics/Bullet.h"
#include "Engine/oop/SceneHandler.h"

#include "game/Ground.h"
#include "game/Player.h"
#include "game/Targets.h"
#include "game/Enemy.h"
#include "game/scenes/CombatScene.h"
#include "game/graphical/BulletHandler.h"

Window* win;
Input::KeyHandler handler;
Camera* camera;

int init() {
    win = new Window("DogFight2077", 2);

    if (!glfwInit()) {
        std::cout << "GLFW error" << std::endl;
        return 0;
    }

    win->Init();

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW error" << std::endl;
        return 0;
    }

    handler.addCallback(GLFW_KEY_F11, GLFW_PRESS, []{
        win->screenUp();
    });

    glfwSetInputMode(win->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(win->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    auto input = [](GLFWwindow* w, int s, int t, int a, int m)
    {
        handler.key_callback(w, s, t, a, m);
    };

    Input::Mouse::init();

    glfwSetKeyCallback(win->window, input);

    //physics
    {
        Bullet.bphase = new btDbvtBroadphase();
        Bullet.colConfig = new btDefaultCollisionConfiguration();
        Bullet.dispatcher = new btCollisionDispatcher(Bullet.colConfig);
        Bullet.solver = new btSequentialImpulseConstraintSolver();
        Bullet.world = new btDiscreteDynamicsWorld(
            Bullet.dispatcher, Bullet.bphase, 
            Bullet.solver, Bullet.colConfig
        );
    }

    glLineWidth(2.0f);
    glPointSize(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.025f, 0.0f, 0.05f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return 1;
}

void loop() {
    TextRenderer* tr = new TextRenderer(win->width, win->height);
    tr->Load("res/fonts/roboto/Roboto-Light.ttf", 128);
    Hud* h = new Hud(win, tr);

    CombatScene * co = new CombatScene(h);
    SceneHandler schandler;
    schandler.tr = tr;
    schandler.changeScene(co);
    schandler.Update();
    camera = co->camera;
    bulletHandler::Init();

    Time::Update();
    while (!glfwWindowShouldClose(win->window))
    {
        OPTICK_FRAME("MainThread")
        {
            OPTICK_GPU_EVENT("Render")
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            schandler.Render();
            bulletHandler::Render(camera->proj, camera->view);
        }

        Time::Update();

        {
            OPTICK_EVENT("UPDATE PHYSICS")
            Bullet.world->stepSimulation(Time::dt);
        }

        {
            OPTICK_EVENT("UPDATE GAME LOGIC")
            schandler.Update();
        }

        {
            OPTICK_GPU_EVENT("BUFFER")
            glfwSwapBuffers(win->window);
        }

        Input::Mouse::delta_x = 0;
        Input::Mouse::delta_y = 0;

        if (Input::getKey(GLFW_KEY_R)) {
            Hud* h = new Hud(win, tr);
            co = new CombatScene(h);
            schandler.changeScene(co);
        }

        glfwPollEvents();
    }

    co->clearScene();
}

int main(void)
{
    if(!init()) 
        return 0;

    loop();

    glfwTerminate();
    return 0;
}