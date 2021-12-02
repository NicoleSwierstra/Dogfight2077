#include "BulletHandler.h"
#include <GL/glew.h>
#include "util/Time.h"

bulletHandler* bulletHandler::bh = new bulletHandler();

void bulletHandler::Init()
{
	bh->vb = new VertexBuffer(nullptr, 0);
	bh->shader = new Shader("res/shaders/lasers.shader");

	std::cout << bh << "\n";
}

void bulletHandler::addBullet(glm::vec3 start, glm::vec3 dir)
{
	bh->bullets.push_back(__BULLET_STRUCT{
		start,
		1.0f,
		start + (dir * 500.0f),
		1.0f
	});
}

void bulletHandler::Render(glm::mat4 proj, glm::mat4 view)
{
	if (bh->bullets.size() == 0) return;

	glLineWidth(3.0f);
	
	bh->vb->Bind();
	bh->vb->Write(&bh->bullets[0], sizeof(__BULLET_STRUCT) * bh->bullets.size());
	bh->shader->Bind();
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 16, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, 0, 16, (void*)12);
	
	bh->shader->SetMat4Uniforms("proj", proj);
	bh->shader->SetMat4Uniforms("view", view);
	
	glDrawArrays(GL_LINES, 0, bh->bullets.size() * 2);
	
	bh->vb->UnBind();
	bh->shader->UnBind();
	glLineWidth(2.0f);

	for (int i = 0; i < bh->bullets.size(); i++) {
		__BULLET_STRUCT* b = &bh->bullets[i];
		b->time -= Time::dt;
		b->time0 = b->time;
		glm::vec3 offset = b->direction - b->origin;
		b->origin += offset * 0.5f * Time::dt;
		if (b->time < 0.0f) {
			bh->bullets.erase(bh->bullets.begin() + i);
		}
	}
}
