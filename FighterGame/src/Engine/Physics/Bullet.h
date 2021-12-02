#pragma once
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

//casting
glm::vec3 toGLM(btVector3& vec);
glm::quat toGLM(btQuaternion& quat);
btVector3 toBT(glm::vec3& vec);
btVector3 toBT(glm::vec2& vec);
btQuaternion toBT(glm::quat& quat);

struct _BULLET_INFO {
	btBroadphaseInterface* bphase;
	btDefaultCollisionConfiguration* colConfig;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
};

extern _BULLET_INFO Bullet;