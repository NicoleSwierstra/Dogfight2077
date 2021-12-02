#include "SceneHandler.h"

Scene* SceneHandler::active, *SceneHandler::queued;

void SceneHandler::internalChange()
{
    if (queued) {
        if (active) {
            active->clearScene();
            delete active;
        }
        active = queued;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        active->currentHud->tr->RenderText("Loading...", -0.85f, 0.65f, 0.001f, glm::vec4(1.0f));
        glfwSwapBuffers(Window::active->window);

        active->init();
        queued = nullptr;
    }
        
}

void SceneHandler::changeScene(Scene* nextscene)
{
    queued = nextscene;
}
