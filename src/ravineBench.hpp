#pragma once

#include <ravine/ecs.h>
#include <vector>

#include "ibenchmark.h"
#include "compTypes.hpp"
#include "systemOneComp.hpp"
#include "systemTwoCompSim.hpp"
#include "systemTwoCompSep.hpp"
#include "systemThreeCompSim.hpp"
#include "systemThreeCompPair.hpp"

using std::vector;
using namespace rv;

class RavineBench : public IBenchmark
{
private:
	vector<Entity> entityStack;
	ISystem* oneCompSystem;
	ISystem* twoCompSepSystem;
	ISystem* twoCompSimSystem;
	ISystem* threeCompSystem;
	ISystem* threeCompFirstSystem;
	ISystem* threeCompSecondSystem;

public:
	inline const char* getName() final
	{
		return "Ravine";
	}

	inline void setupOneComp(int entityCount) final
	{
		oneCompSystem = new OneCompSystem();
		for (int i = 0; i < entityCount; i++)
		{
			entityStack.push_back(EntitiesManager::createEntity<CompA>());
		}
	}
	inline void setupTwoCompSim(int entityCount) final
	{
		twoCompSimSystem = new TwoCompSimSystem();
		for (int i = 0; i < entityCount; i++)
		{
			entityStack.push_back(EntitiesManager::createEntity<CompA, CompB>());
		}
	}
	inline void setupTwoCompSep(int entityCount) final
	{
		twoCompSepSystem = new TwoCompSepSystem();
		for (int i = 0; i < entityCount; i++)
		{
			entityStack.push_back(EntitiesManager::createEntity<CompA, CompB>());
		}
	}
	inline void setupThreeComp(int entityCount) final
	{
		threeCompSystem = new ThreeCompSystem();
		for (int i = 0; i < entityCount; i++)
		{
			entityStack.push_back(EntitiesManager::createEntity<CompA, CompB, CompC>());
		}
	}
	inline void setupThreeCompPair(int entityCount) final
	{
		threeCompFirstSystem = new ThreeCompFirstSystem();
		threeCompSecondSystem = new ThreeCompSecondSystem();
		for (int i = 0; i < entityCount; i++)
		{
			entityStack.push_back(EntitiesManager::createEntity<CompA, CompB, CompC>());
		}
	}

	inline void tickOneComp(double deltaTime) final
	{
		oneCompSystem->update(deltaTime);
	}
	inline void tickTwoCompSim(double deltaTime) final
	{
		twoCompSimSystem->update(deltaTime);
	}
	inline void tickTwoCompSep(double deltaTime) final
	{
		twoCompSepSystem->update(deltaTime);
	}
	inline void tickThreeComp(double deltaTime) final
	{
		threeCompSystem->update(deltaTime);
	}
	inline void tickThreeCompPair(double deltaTime) final
	{
		threeCompFirstSystem->update(deltaTime);
		threeCompSecondSystem->update(deltaTime);
	}

	inline void cleanup() final
	{
		if (oneCompSystem != NULL) delete oneCompSystem; oneCompSystem = NULL;
		if (twoCompSepSystem != NULL) delete twoCompSepSystem; twoCompSepSystem = NULL;
		if (twoCompSimSystem != NULL) delete twoCompSimSystem; twoCompSimSystem = NULL;
		if (threeCompSystem != NULL) delete threeCompSystem; threeCompSystem = NULL;
		if (threeCompFirstSystem != NULL) delete threeCompFirstSystem; threeCompFirstSystem = NULL;
		if (threeCompSecondSystem != NULL) delete threeCompSecondSystem; threeCompSecondSystem = NULL;

		size_t size = entityStack.size();
		while (size > 0)
		{
			EntitiesManager::removeEntity(entityStack[size - 1]);
			entityStack.pop_back();
			size--;
		}
		entityStack.clear();
	}
};