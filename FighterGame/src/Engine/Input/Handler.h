#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>

namespace Input {
	using Callback = std::function<void()>;

	class KeyHandler {
	private:
		//I think this makes 3 seperate maps for the 3 kinds of actions
		std::map<int, std::vector<Callback>> callbacks[3]; 
	public:
		//the type is either GLFW_RELEASE | GLFW_PRESS | GLFW_REPEAT
		void addCallback(int key, int type, Callback c);

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

	class Mouse {
		static double lastxp, lastyp;
	public:
		//[-1, 1]
		static double pos_x, pos_y;
		static double delta_x, delta_y;

		static void init();
	};

	bool getKey(int key);

	float getAxis(int key1, int key2);

	bool getMouseButton(int button);
}

