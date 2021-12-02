#include "Window.h"
#include <iostream>

Window* Window::active;

Window::Window(const std::string& name, int screentype)
{
    active = this;
    this->name = name;
    screen = screentype;
}

void Window::window_size_callback(GLFWwindow* window, int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    aspect = (float)width / (float)height;
}

std::string Window::resDown()
{
    if (fs.i == 0) goto h;
    fs.i--;
    width = fs.sizes[fs.i].x;
    height = fs.sizes[fs.i].y;
    aspect = (float)width / (float)height;
    
    glViewport(0, 0, width, height);
    glfwSetWindowSize(window, width, height);
    
    h:
    return std::to_string(width) + " x " + std::to_string(height);
}

std::string Window::resUp()
{
    if (fs.i == 9) goto h;
    fs.i++;
    width = fs.sizes[fs.i].x;
    height = fs.sizes[fs.i].y;
    aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glfwSetWindowSize(window, width, height);

    h:
    return std::to_string(width) + " x " + std::to_string(height);
}

void Window::Init() {
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_SAMPLES, 16);

    width = mode->width;
    height = mode->height;
    aspect = (float)width / (float)height;
    vsync = false;

    glfwWindowHint(GLFW_DECORATED, 1);
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
    }

    while (fs.i < 10) {
        if (fs.sizes[fs.i].x == width || fs.sizes[fs.i].y == height) break;
        fs.i++;
    }

    GLFWwindowsizefun winsize = [](GLFWwindow* window, int width, int height) {
        Window::active->width = width; Window::active->height = height;
        Window::active->aspect = (float)width / (float)height;
    };

    glfwSetWindowSizeCallback(window, winsize);

    glfwMakeContextCurrent(window);
    applyscreen();
    glfwSwapInterval(1);
}

void Window::applyscreen() {
    switch (screen) {
        case 0: {
            glfwSetWindowMonitor(window, monitor, 0, 0, width, height, mode->refreshRate);
        } break;
        case 1: {
            glfwSetWindowAttrib(window, GLFW_DECORATED, 0);
            glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, mode->refreshRate);
        } break;
        case 2: {
            glfwSetWindowAttrib(window, GLFW_DECORATED, 1);
            int x, y;
            glfwGetWindowPos(window, &x, &y);
            glfwSetWindowMonitor(window, nullptr, x, y, width, height, mode->refreshRate);
            glfwMaximizeWindow(window);
        }
    }
}

void Window::screenUp() {
    screen++;
    screen %= 3;
    applyscreen();
}
void Window::screenDown() {
    screen += 2;
    screen %= 3;
    applyscreen();
}

void Window::toggleVsync() {
    vsync = !vsync;
    glfwSwapInterval(vsync);
}

std::string Window::applyAA() {
    switch (aamodes.i) {
        case 0:
            glDisable(GL_MULTISAMPLE);
            break;
        case 1:
            glEnable(GL_MULTISAMPLE);
            glfwWindowHint(GL_SAMPLES, 2);
            break;
        case 2:
            glEnable(GL_MULTISAMPLE);
            glfwWindowHint(GL_SAMPLES, 4);
            break;
        case 3:
            glEnable(GL_MULTISAMPLE);
            glfwWindowHint(GL_SAMPLES, 8);
            break;
        case 4:
            glEnable(GL_MULTISAMPLE);
            glfwWindowHint(GL_SAMPLES, 16);
            break;
    }
    
    //broken AA apply code:

    //glfwDestroyWindow(window);
    //window = glfwCreateWindow(width, height, name.c_str(), 0, 0);
    //glfwMakeContextCurrent(window);
    //applyscreen();
    return aamodes.modes[aamodes.i];
}

std::string Window::AAup() {
    aamodes.i++;
    aamodes.i %= 5;
    return applyAA();
}
std::string Window::AAdown() {
    aamodes.i += 4;
    aamodes.i %= 5;
    return applyAA();
}


