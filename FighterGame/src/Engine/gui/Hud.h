#pragma once
#include "TextRenderer.h"
#include "Window.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/EBO.h"
#include <vector>
#include <functional>
#include "graphics/Texture.h"

struct textInstance {
	float* fp;
	const char* cp;
	int* ip;
	float x, y, scale;
};

struct rectInstance {
	float left, right, top, bottom;
	glm::vec4 color;
};

struct texInstance {
	float left, right, top, bottom;
	Texture* tex;
};

struct buttonInstance {
	float left, right, top, bottom;
	std::string text;
	glm::vec4 color;
	glm::vec4 hoverColor;
	glm::vec4 textColor;
	std::function<void()> func;
};

struct checkBoxInstance {
	float left, right, top, bottom;
	std::string text;
	glm::vec4 color;
	glm::vec4 textColor;
	bool enabled;
	std::function<void()> enablefunc;
	std::function<void()> disablefunc;
};

class Hud {
	Window* win;
	std::vector<textInstance*> ti;
	std::vector<rectInstance*> ri;
	std::vector<texInstance*> texi;
	std::vector<buttonInstance*> bi;
	std::vector<checkBoxInstance*> cbi;
	VertexBuffer* vb;
	Shader* rectShader, * texShader;
	EBO* ebo;
public:
	TextRenderer* tr;
	~Hud();
	Hud(Window* w, TextRenderer* tr);

	//add a pointer to a float
	textInstance* AddText(float* text, float x, float y, float scale);
	//add a pointer to an int
	textInstance* AddText(int* text, float x, float y, float scale);
	//add a string with c_str()
	textInstance* AddText(const char* text, float x, float y, float scale);

	//rectangle?
	rectInstance* AddRect(float left, float right, float top, float bottom, glm::vec4 color);
	
	//textured rectangle
	texInstance* AddTexture(float left, float right, float top, float bottom, std::string texture);

	//button i hope
	buttonInstance* AddButton(float left, float right, float top, float bottom, glm::vec4 color, glm::vec4 hoverColor, glm::vec4 textColor, std::string text, std::function<void()> func);

	checkBoxInstance* AddCheckBox(float left, float right, float top, float bottom, std::string text, glm::vec4 color, glm::vec4 textColor, std::function<void()> enable, std::function<void()> disable);

	//call this as the last stage each frame
	void Render();
	void CheckButtons(double x, double y);
};