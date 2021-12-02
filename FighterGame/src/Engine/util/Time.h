#pragma once
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <vector>

struct timerNodes {
	float time;
	std::function<void()> func;
};

//static classes kinda suck
class Time {
	static std::chrono::steady_clock::time_point st;
	static float _sec;
	static int _fr;
	static double _pausetime;
	static bool _paused;
	static std::vector<timerNodes> _tn;

public:
	static float dt;
	static float usdt;
	static int fps;
	static float scale;
	static float elapsed;
	static float elapsedUnscaled;

	static void Update();
	static void Pause();
	static void Pause(bool pause);
	//adds a timer to call a function in time number of seconds
	static void addTimer(std::function<void()> func, float time);
	static bool isPaused();
};