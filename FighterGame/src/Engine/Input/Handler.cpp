#include "Handler.h"
#include "Window.h"
#include <iostream>

namespace Input {
	double Mouse::lastxp, Mouse::lastyp;
	double Mouse::pos_x, Mouse::pos_y;
	double Mouse::delta_x, Mouse::delta_y;

	void KeyHandler::addCallback(int key, int type, Callback c)
	{
		callbacks[type][key].push_back(c);
	}

	void KeyHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(callbacks[action].count(key) > 0)
			for (Callback c : callbacks[action][key]) c();
	}

	void Mouse::init() {
		glfwGetCursorPos(Window::active->window, &lastxp, &lastyp);
		auto mouse_callback = [](GLFWwindow* w, double xpos, double ypos) {
			pos_x = xpos;
			pos_y = ypos;

			delta_x = pos_x - lastxp;
			delta_y = -(pos_y - lastyp);
			
			lastxp = xpos;
			lastyp = ypos;
		};
		glfwSetCursorPosCallback(Window::active->window, mouse_callback);
	}

	bool getKey(int keycode) {
		return glfwGetKey(Window::active->window, keycode);
	}

	//returns [-1, 1] between the two keys
	float getAxis(int key1, int key2) {
		return (float)getKey(key1) - (float)getKey(key2);
	}

	bool getMouseButton(int button) {
		return glfwGetMouseButton(Window::active->window, button);
	}
}
