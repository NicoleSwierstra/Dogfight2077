#include "Hud.h"
#include <iostream>

Hud::Hud(Window* w, TextRenderer* tr) {
	win = w;
	this->tr = tr;
	vb = new VertexBuffer(nullptr, 16 * sizeof(float));
	rectShader = new Shader("res/shaders/hud.shader");
	texShader = new Shader("res/shaders/hudTex.shader");
	ebo = new EBO(EBO_QUAD);
}

textInstance* Hud::AddText(float* text, float x, float y, float scale)
{
	textInstance* t = new textInstance{ text, nullptr, nullptr, x, y, scale };
	ti.push_back(t);
	return t;
}

textInstance* Hud::AddText(int* text, float x, float y, float scale)
{
	textInstance* t = new textInstance{ nullptr, nullptr, text, x, y, scale };
	ti.push_back(t);
	return t;
}

textInstance* Hud::AddText(const char* text, float x, float y, float scale)
{
	textInstance* t = new textInstance{ nullptr, text, nullptr, x, y, scale };
	ti.push_back(t);
	return t;
}

std::string rmdec(std::string source, char sf) {
	int i = 0;
	while (source[i] != 0 && source[i] != '.') i++;
	if (source[i] == 0) return source;
	i += sf + 1;
	return source.substr(0, i);
}

texInstance* Hud::AddTexture(float left, float right, float top, float bottom, std::string texture) {
	texInstance* tex = new texInstance{left, right, top, bottom, new Texture(texture)};
	texi.push_back(tex);
	return tex;
}

rectInstance* Hud::AddRect(float left, float right, float top, float bottom, glm::vec4 color) {
	rectInstance* r = new rectInstance{ left, right, top, bottom, color };
	ri.push_back(r);
	return r;
}

buttonInstance* Hud::AddButton(float left, float right, float top, float bottom, glm::vec4 color, glm::vec4 hoverColor, glm::vec4 textColor, std::string text, std::function<void()> func)
{
	buttonInstance *b = new buttonInstance{ left, right, top, bottom, text, color, hoverColor, textColor, func };
	bi.push_back(b);
	return b;
}

checkBoxInstance* Hud::AddCheckBox(float left, float right, float top, float bottom, std::string text, glm::vec4 color, glm::vec4 textColor, std::function<void()> enable, std::function<void()> disable)
{
	checkBoxInstance* c = new checkBoxInstance{ left, right, top, bottom, text, color, textColor, false, enable, disable };
	cbi.push_back(c);
	return c;
}

void Hud::Render()
{
	glDisable(GL_DEPTH_TEST);
	ebo->Bind();
	rectShader->Bind();
	vb->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);
	for (int i = 0; i < ri.size(); i++) {
		float vs[8] = {
			ri[i]->left, -ri[i]->bottom,
			ri[i]->right, -ri[i]->bottom,
			ri[i]->left, -ri[i]->top,
			ri[i]->right, -ri[i]->top
		};

		rectShader->SetV4Uniforms("in_color", ri[i]->color);
		vb->Write(vs, 8 * sizeof(float));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < ti.size(); i++) {
		std::string h;
		if (ti[i]->cp != nullptr) {
			h = ti[i]->cp;
		}
		else if (ti[i]->fp != nullptr) {
			h = rmdec(std::to_string(*ti[i]->fp), 1);
		}
		else if (ti[i]->ip != nullptr) {
			h = std::to_string(*ti[i]->ip);
		}
		else __debugbreak();
		tr->RenderText(h, ti[i]->x, ti[i]->y, ti[i]->scale, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	
	for (int i = 0; i < bi.size(); i++) {
		ebo->Bind();
		rectShader->Bind();
		vb->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);
		float vs[8] = {
			bi[i]->left, -bi[i]->bottom,
			bi[i]->right, -bi[i]->bottom,
			bi[i]->left, -bi[i]->top,
			bi[i]->right, -bi[i]->top
		};

		rectShader->SetV4Uniforms("in_color", bi[i]->color);
		vb->Write(vs, 8 * sizeof(float));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		tr->RenderText(bi[i]->text, bi[i]->left, bi[i]->top, (bi[i]->bottom - bi[i]->top)/128.0f, bi[i]->textColor);
	}

	for (int i = 0; i < cbi.size(); i++) {
		float tb = (cbi[i]->bottom - cbi[i]->top);
		ebo->Bind();
		rectShader->Bind();
		vb->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);
		float vs[8] = {
			cbi[i]->left, -cbi[i]->bottom,
			cbi[i]->left + (tb * 0.5625), -cbi[i]->bottom,
			cbi[i]->left, -cbi[i]->top,
			cbi[i]->left + (tb * 0.5625), -cbi[i]->top
		};

		rectShader->SetV4Uniforms("in_color", cbi[i]->textColor);
		vb->Write(vs, 8 * sizeof(float));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		if (!cbi[i]->enabled) {
			float offset = 0.005f, xoffset = offset / win->aspect;
			float vs[8] = {
				cbi[i]->left + xoffset, -cbi[i]->bottom + offset,
				cbi[i]->left + (tb * 0.5625) - xoffset, -cbi[i]->bottom + offset,
				cbi[i]->left + xoffset, -cbi[i]->top - offset,
				cbi[i]->left + (tb * 0.5625) - xoffset, -cbi[i]->top - offset
			};

			rectShader->SetV4Uniforms("in_color", cbi[i]->color);
			vb->Write(vs, 8 * sizeof(float));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		tr->RenderText(cbi[i]->text, cbi[i]->left + tb, cbi[i]->top, tb / 128.0f, cbi[i]->textColor);
	}

	texShader->Bind();
	ebo->Bind();
	vb->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 16, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 16, (void*)8);
	for (int i = 0; i < texi.size(); i++) {
		float vs[16] = {
			texi[i]->left,  -texi[i]->bottom, 0.0f, 0.0f,
			texi[i]->right, -texi[i]->bottom, 1.0f, 0.0f,
			texi[i]->left,  -texi[i]->top, 0.0f, 1.0f,
			texi[i]->right, -texi[i]->top, 1.0f, 1.0f
		};
		texi[i]->tex->Bind();
		texShader->SetIntUniforms("tex", 0);
		vb->Write(vs, 16 * sizeof(float));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glEnable(GL_DEPTH_TEST);
}

void Hud::CheckButtons(double x, double y)
{
	for (int i = 0; i < bi.size(); i++) {
		if (x > bi[i]->left && x < bi[i]->right && y < bi[i]->bottom && y > bi[i]->top) {
			bi[i]->func();
			return;
		}
	}
	for (int i = 0; i < cbi.size(); i++) {
		if (x > cbi[i]->left && x < cbi[i]->right && y < cbi[i]->bottom && y > cbi[i]->top) {
			cbi[i]->enabled = !cbi[i]->enabled;
			cbi[i]->enabled ? cbi[i]->enablefunc() : cbi[i]->disablefunc();
			return;
		}
	}
}

Hud::~Hud() {
	for (textInstance * i : ti)
		delete i;
	for (buttonInstance* i : bi)
		delete i;
	for (rectInstance* i : ri)
		delete i;
	for (checkBoxInstance * i : cbi)
		delete i;
	for (texInstance* i : texi)
		delete i;
}