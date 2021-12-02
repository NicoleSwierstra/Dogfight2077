#include "Time.h"
#include <optick.h>

std::chrono::steady_clock::time_point Time::st = std::chrono::steady_clock::now();
float Time::_sec = 0.0f;
int Time::_fr = 0;
double Time::_pausetime = 0;
bool Time::_paused = 0;
std::vector<timerNodes> Time::_tn;

float Time::dt = 0;
float Time::usdt = 0;
int Time::fps = 0;
float Time::scale = 1.0f;
float Time::elapsed = 0.0f;
float Time::elapsedUnscaled = 0.0f;

void Time::Update() {
	OPTICK_EVENT();
	float ift = (
		(std::chrono::nanoseconds)(std::chrono::steady_clock::now() - st)
		).count() * 0.000000001f;
	usdt = ift;
	dt = ift * scale;
	elapsed += dt;
	elapsedUnscaled += ift;
	st = std::chrono::steady_clock::now();
	_sec += ift;
	if (_sec > 1.0f) {
		_sec -= 1.0f;
		fps = _fr;
		_fr = 0;
	}
	_fr++;
	for (int i = 0; i < _tn.size(); i++) {
		_tn[i].time -= dt;
		if (_tn[i].time < 0) { 
			_tn[i].func(); 
			_tn.erase(_tn.begin() + i); 
		}
	}
}

void Time::Pause() {
	if (!_paused) {
		scale = 0.0f;
		_pausetime = glfwGetTime();
		_paused = true;
	}
	else {
		scale = 1.0f;
		_paused = false;
		glfwSetTime(_pausetime);
	}
}
void Time::Pause(bool pause) {
	if (pause) {
		scale = 0.0f;
		_paused = true;
		_pausetime = glfwGetTime();
	}
	else {
		scale = 1.0f;
		_paused = false;
		glfwSetTime(_pausetime);
	}
}

void Time::addTimer(std::function<void()> func, float time)
{
	_tn.push_back(timerNodes{time, func});
}

bool Time::isPaused() {
	return _paused;
}