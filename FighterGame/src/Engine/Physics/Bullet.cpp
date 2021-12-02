#include "Bullet.h"

_BULLET_INFO Bullet;

glm::vec3 toGLM(btVector3& vec) {
	return glm::vec3(vec.x(), vec.y(), vec.z());
};

btVector3 toBT(glm::vec3& vec) {
	return btVector3(vec.x, vec.y, vec.z);
};

btVector3 toBT(glm::vec2& vec) {
	return btVector3(vec.x, vec.y, 0.0f);
};

glm::quat toGLM(btQuaternion& quat) {
	return glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
};

btQuaternion toBT(glm::quat& quat) {
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
};