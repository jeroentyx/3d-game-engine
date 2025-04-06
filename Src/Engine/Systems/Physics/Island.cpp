#include "Island.h"

void Island::addBody(std::shared_ptr<RigidBody> body) {
	if (i_bodyCount < i_maxBodyCount) {
		i_bodies[i_bodyCount++] = body;
	}
}

void Island::initialize() {
	//once learned about contacts
}

void Island::solve() {
	////gravity
	//for (int i = 0; i < i_bodyCount; ++i) {
	//	std::shared_ptr<RigidBody> body = i_bodies[i];
	//	VelocitySaved* v = i_velocities + i++;
	//	if (body->getBodyType() == BodyType::DYNAMIC) {
	//		body->applyForce(gravity * body->getGravityDirection());

	//		//vel
	//		body->setLinearVelocity(body->getLinearVelocity() + (body->getForce() * body->getInvMass() * dt));
	//	}
	//	v->v = body->getLinearVelocity();
	//	v->w = body->getAngularVelocity();
	//}
}