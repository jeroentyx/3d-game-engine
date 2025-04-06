#include "Scene.h"

#define MAX_ITERATIONS 1

Scene::Scene(): s_gravity{9.81f},
                s_sleep{true},
		        s_friction{true},
				s_update{true},
				s_firstCollisionCheck{ false }
{
}

Scene::Scene(const float& g)
	: s_gravity{ g },
	s_sleep{ true },
	s_friction{ true },
	s_update{ true },
	s_firstCollisionCheck{ false }{

}

Scene::~Scene() {
	RemoveAllBodies();
}

std::queue<std::pair<uint32_t, uint32_t>> Scene::getSurgeQueue() {
	return s_surgeQueue;
}

void Scene::addToSurgeQueue(std::pair<uint32_t, uint32_t> s_pair) {
	s_surgeQueue.push(s_pair);
}

void Scene::clearSurgeQueue() {
	std::queue<std::pair<uint32_t, uint32_t>> empty;
	std::swap(s_surgeQueue, empty);
}

std::vector<Manifold> Scene::NarrowPhase(float _dt) {
	UNREFERENCED_PARAMETER(_dt);
	std::vector<Manifold> manifolds;
	for (auto it = s_bodies.begin(); it != s_bodies.end(); ++it) {
		for (auto it2 = it + 1; it2 != s_bodies.end(); ++it2) {
			if ((*it)->getBodyType() == BodyType::STATIC && (*it2)->getBodyType() == BodyType::STATIC)
				continue;

			Manifold manifold;
			
			if (DynamicAABBTest((*it)->getAABB(), (*it)->getLinearVelocity(), (*it2)->getAABB(), (*it2)->getLinearVelocity(), manifold)) {
				manifold.m_a = (*it);
				manifold.m_b = (*it2);
				manifold.m_friction = ((*it)->getFriction() + (*it2)->getFriction()) * 0.5f;
				manifolds.push_back(manifold);

			}
			else {

				if ((*it)->isEntityInvolved((*it2)->rb_id)) {
					if ((*it)->getStatus((*it2)->rb_id) == CollidingStatus::EXIT) {		//exited collision remove from vector
						(*it)->removeCollidingEntity((*it2)->rb_id);
						(*it2)->removeCollidingEntity((*it)->rb_id);
					}
					else if ((*it)->getStatus((*it2)->rb_id) == CollidingStatus::NONE) {
						(*it)->removeCollidingEntity((*it2)->rb_id);
						(*it2)->removeCollidingEntity((*it)->rb_id);
					}
					else if ((*it)->getStatus((*it2)->rb_id) == CollidingStatus::ENTER || (*it)->getStatus((*it2)->rb_id) == CollidingStatus::STAY){																//enter/stay collision exited in next frame
						(*it)->setCollidingStatus((*it2)->rb_id, CollidingStatus::EXIT);
						(*it2)->setCollidingStatus((*it)->rb_id, CollidingStatus::EXIT);
						if (ScriptSystem::IsEntityModuleValid(EntityID{ (*it)->rb_id , 0 }))
							ScriptSystem::OnTriggerEnd((*it)->rb_id);
						if (ScriptSystem::IsEntityModuleValid(EntityID{ (*it2)->rb_id , 0 }))
							ScriptSystem::OnTriggerEnd((*it2)->rb_id);
					}
				}

			}
		}
	}
	return manifolds;
}

void Scene::Solve(float dt) {
	for (auto& body : s_bodies) {
		if (body->getBodyType() == BodyType::DYNAMIC || body->getBodyType() == BodyType::KINEMATIC) {
			if (!body->getCollisionFlag()) {
				body->setPrevPosition(body->getPosition());

				//grav
				if (body->getGravityFlag()) {
					body->applyGravity(s_gravity, dt);
					body->setPrevPosition(body->getPosition());
				}
				else {
					body->setLinearVelocity(glm::vec3{ body->getLinearVelocity().x, 0, body->getLinearVelocity().z });
				}

				//v1 = v0 + a * dt
				body->setLinearVelocity(body->getLinearVelocity() + body->getAcceleration() * dt);
				body->setFrameOffset(body->getLinearVelocity() * dt);
				body->setPosition(body->getPosition() + body->getLinearVelocity() * dt);
				
			}
			else {
				body->setPrevPosition(body->getPosition());

				//grav
				if (body->getGravityFlag()) {
					body->applyGravity(s_gravity, dt);
					body->setPrevPosition(body->getPosition());
				}
				else {
					body->setLinearVelocity(glm::vec3{ body->getLinearVelocity().x, 0, body->getLinearVelocity().z });
				}

				//v1 = v0 + a * dt
				body->setLinearVelocity(body->getLinearVelocity() + body->getAcceleration() * body->getSolveTime());
				body->setFrameOffset(body->getLinearVelocity() * dt);
				body->setPosition(body->getPosition() + body->getLinearVelocity() * body->getSolveTime());
			}
			body->setRotation(body->getRotation() + body->getAngularVelocity() * (0.5f) * dt);
		}

		//update AABB here//
		//for now all rb have aabb
		body->updateAABB();
		body->setMass();
		body->setFrameOffset(glm::vec3());
	}

	//std::cout << "(SOLVE)UPDATED" << std::endl;
	//clear forces
	for (auto& body : s_bodies) {
		body->resetForce();
		body->resetTorque();
	}
}

void Scene::Step(float dt) {
	clearSurgeQueue();
	s_firstCollisionCheck = false;
	for (auto& it : s_bodies) {
		it->setBodyTypeInternal();
		it->setCollisionFlag(false);
		it->setPreviousCollisionFlag(it->getCurrentCollisionFlag());
		it->setCurrentCollisionFlag(false);
		it->setSolveTime(dt);
		if (it->getFrictionFlag()) {
			it->applyFriction();
		}
		it->setAngularVelocity(it->getAngularVelocity() * 0.99f);
	}

	//make island next time then make into manifolds

	//loop many times if needed but lets keep it at 1 for now
	//any more than 2 and weird bounce/yeet (definitely undesirable)

	for (int i = 0; i < MAX_ITERATIONS; ++i) {
		std::vector<Manifold> manifolds = NarrowPhase(dt);
		if (!manifolds.empty()) {
			for (auto& it : manifolds) {
				ResolveDynamicManifold(it, dt, s_friction, s_surgeQueue);
				triggerManifoldScripts(it);
			}
		}
	}

	s_firstCollisionCheck = true;

	//apply acceleration->vel->pos here
	Solve(dt);
}

std::shared_ptr<RigidBody> Scene::CreateBody() {
	auto body = std::make_shared<RigidBody>();
	s_bodies.push_back(body);
	return body;
}

void Scene::triggerManifoldScripts(const Manifold& manifold) {
	std::shared_ptr<RigidBody> a = manifold.m_a;
	std::shared_ptr<RigidBody> b = manifold.m_b;

	//collision alr detected on prev frames
	if ((a)->isEntityInvolved((b)->rb_id)) {
		//if collision is stayed prev frame, just trigger stay again
		if ((a)->getStatus((b)->rb_id) == CollidingStatus::STAY) {
		if (ScriptSystem::IsEntityModuleValid(EntityID{ (a)->rb_id , 0 }))
			ScriptSystem::OnTriggerStay((a)->rb_id);
		if (ScriptSystem::IsEntityModuleValid(EntityID{ (b)->rb_id , 0 }))
			ScriptSystem::OnTriggerStay((b)->rb_id);
		}
		//collision already occured previous frame, now is stay frame
		else if ((a)->getStatus((b)->rb_id) == CollidingStatus::ENTER) { 
			(a)->setCollidingStatus((b)->rb_id, CollidingStatus::STAY);
			(b)->setCollidingStatus((a)->rb_id, CollidingStatus::STAY);
			if (ScriptSystem::IsEntityModuleValid(EntityID{ (a)->rb_id , 0 }))
				ScriptSystem::OnTriggerStay((a)->rb_id);
			if (ScriptSystem::IsEntityModuleValid(EntityID{ (b)->rb_id , 0 }))
				ScriptSystem::OnTriggerStay((b)->rb_id);
		}
		//collision either exiting or didnt exist but is somehow not removed from vector, set to enter
		else if ((a)->getStatus((b)->rb_id) == CollidingStatus::EXIT || (a)->getStatus((b)->rb_id) == CollidingStatus::NONE) {
			(a)->setCollidingStatus((b)->rb_id, CollidingStatus::ENTER);
			(b)->setCollidingStatus((a)->rb_id, CollidingStatus::ENTER);
			if (ScriptSystem::IsEntityModuleValid(EntityID{ (a)->rb_id , 0 }) &&
				ScriptSystem::IsEntityModuleValid(EntityID{ (b)->rb_id , 0 }))
			{
				ScriptSystem::OnTriggerBegin((a)->rb_id, (b)->rb_id);
			}
		}
		
	}
	//collision happening for the first time
	else {
		(a)->addCollidingEntity((b)->rb_id, CollidingStatus::ENTER); //fresh entered collision
		(b)->addCollidingEntity((a)->rb_id, CollidingStatus::ENTER);
		if (ScriptSystem::IsEntityModuleValid(EntityID{ (a)->rb_id , 0 }) &&
			ScriptSystem::IsEntityModuleValid(EntityID{ (b)->rb_id , 0 }))
		{
			ScriptSystem::OnTriggerBegin((a)->rb_id, (b)->rb_id);
		}
	}

}

void Scene::AddBody(const std::shared_ptr<RigidBody>& body) {
	if (!std::count(s_bodies.begin(), s_bodies.end(), body))
		s_bodies.push_back(body);
}

void Scene::RemoveBody(const std::shared_ptr<RigidBody>& body) {
	//c++20 implementation
	std::erase(s_bodies, body);
}

void Scene::RemoveAllBodies() {
	s_bodies.clear();
}

void Scene::setSleep(bool ean) {
	s_sleep = ean;
}

void Scene::setFriction(bool ean) {
	s_friction = ean;
}

void Scene::setUpdate(bool ean) {
	s_update = ean;
}

void Scene::setGravity(const float& g) {
	s_gravity = g;
}