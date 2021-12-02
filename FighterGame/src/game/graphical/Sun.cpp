#include "Sun.h"
#include <vector>
#include "oop/SceneHandler.h"
#include "Cameras/ChaseCam.h"
#include "util/Time.h"

const int divisions = 30;
const double pioverdiv = 6.28318530718 / divisions;
const int rings = 10;
const float scale = 1.0f;

GraphicsSun::GraphicsSun() {
	sunShader = new Shader("res/shaders/sun.shader");
	vb = new VertexBuffer * [3];
	
	glm::vec3 *verts = new glm::vec3[divisions + 1];
	std::vector<int> _ebo;

	for (int i = 1; i <= divisions; i++) {
		double theta = pioverdiv * (i - 1);
		verts[i] = glm::vec3(cos(theta) * scale, sin(theta) * scale, 0.0f);
		_ebo.insert(_ebo.end(), {i, 0, i+1});
	}
	
	_ebo[_ebo.size() - 1] = 1; //make it a loop
	ebo = new EBO(&_ebo, sizeof(int) * _ebo.size());

	vb[0] = new VertexBuffer(verts, (divisions + 1) * sizeof(glm::vec3));
	delete[] verts;


	std::vector<glm::vec3> verts1;
	std::vector<glm::vec3> verts2;
	
	for (int ring = 8; ring < 24; ring++) {
		float loc_scale = scale * (ring * 0.03125f);
		float loc_piod = 6.28318530718 / ring;
		if (ring % 2 == 0) {
			for (int i = 0; i <= ring; i++) {
				double theta = loc_piod * i;
				verts2.push_back(glm::vec3(cos(theta) * loc_scale, sin(theta) * loc_scale, 0.0f));
			}
		}
		else {
			for (int i = 0; i <= ring; i++) {
				double theta = loc_piod * i;
				verts1.push_back(glm::vec3(cos(theta) * loc_scale, sin(theta) * loc_scale, 0.0f));
			}
		}
	}
	vb[1] = new VertexBuffer(verts1.data(), verts1.size() * sizeof(glm::vec3));
	vb[2] = new VertexBuffer(verts2.data(), verts2.size() * sizeof(glm::vec3));
}

void GraphicsSun::Update()
{
}

void GraphicsSun::Render()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 16, 0);
	ChaseCam* cam = (ChaseCam*)SceneHandler::active->camera;
	glm::mat4 view(glm::translate(cam->view, *cam->target));
	sunShader->SetMat4Uniforms("proj", cam->proj);
	sunShader->SetMat4Uniforms("view", view);

	vb[0]->Bind();

	sunShader->SetFloatUniforms("time", Time::elapsed);
	sunShader->SetFloatUniforms("time", -Time::elapsed);
}