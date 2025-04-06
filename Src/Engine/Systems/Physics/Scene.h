#pragma once
#include<iostream>
#include "../Src/pch.h"
#include "RigidBody.h"
#include <vector>
#include "Cube.h"
#include "Square.h"
#include "Island.h"
#include "ColliderBox.h"
#include <queue>

#ifndef SCENE_H
#define SCENE_H
#include "Manifold.h"
#include "CollisionManager.h"
#include "Src/Engine/Systems/Logic/ScriptingSystem.hpp"

class Scene {
public:
	Scene();
	Scene(const float& g);
	~Scene();

	std::queue<std::pair<uint32_t, uint32_t>> getSurgeQueue();
	void addToSurgeQueue(std::pair<uint32_t, uint32_t> s_pair);
	std::vector<Manifold> NarrowPhase(float dt);
	void clearSurgeQueue();
	void Solve(float dt);
	void Step(float dt);
	std::shared_ptr<RigidBody> CreateBody();
	void triggerManifoldScripts(const Manifold& manifold);
	
	
	void AddBody(const std::shared_ptr<RigidBody>& body);
	void RemoveBody(const std::shared_ptr<RigidBody>& body);
	void RemoveAllBodies();

	void setSleep(bool ean);
	void setFriction(bool ean);
	void setUpdate(bool ean);

	//void setGravity(const glm::vec3& g);
	void setGravity(const float& g);
private:
	//Island island;
	std::vector<std::shared_ptr<RigidBody>> s_bodies;
	std::queue<std::pair<uint32_t, uint32_t>> s_surgeQueue;
	float s_gravity;
	bool s_sleep;
	bool s_friction;
	bool s_update;
	bool s_firstCollisionCheck;
};

#endif // MANIFOLD_H