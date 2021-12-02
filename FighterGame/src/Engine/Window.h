#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class framesizes {
public:
	int i = 5;
	const glm::ivec2 sizes[10] = {
		glm::vec2( 640,  360),
		glm::vec2( 800,  600),
		glm::vec2(1024,  768),
		glm::vec2(1280,  720),
		glm::vec2(1600,  900),
		glm::vec2(1920, 1080),
		glm::vec2(2560, 1080),
		glm::vec2(2560, 1440),
		glm::vec2(3440, 1440),
		glm::vec2(3840, 2160)
	};
};

class AAmodes {
public:
	int i = 4;
	const std::string modes[5] = {
		"None",
		"2x MSAA",
		"4x MSAA",
		"8x MSAA",
		"16x MSAA"
	};
};

class Window {
public:
	static Window* active;
private:
	const GLFWvidmode* mode;
	GLFWmonitor* monitor;
	std::string name;
	framesizes fs;
	AAmodes aamodes;
	void applyscreen();
public:
	GLFWwindow* window;
	int width, height;
	float aspect; //aspect ratio
	unsigned char screen; //0 = fullscreen, 1 = borderless, 2 = windowed
	bool vsync; //is vsync on???

	const std::string  screennames[3] = {"fullscreen","borderless","windowed"};

	Window(const std::string& name, int screentype = 0);
	//CALL AFTER GLFW INIT OR IT WILL CRASH!
	void Init();
	void screenUp(), screenDown();
	void toggleVsync();
	std::string applyAA();
	std::string AAup();
	std::string AAdown();
	void window_size_callback(GLFWwindow* window, int width, int height);
	std::string resUp(), resDown();
};