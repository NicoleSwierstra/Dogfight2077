#include "GameObject.h"
#include "glm/gtc/matrix_transform.hpp"


void GameObject::updateTrans()
{
	trans = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(rot);
}
