#pragma once
// THIS SYSTEM UPDATES 2 COMPONENT TYPES SIMULTANEOUSLY

#include <ravine/ecs.h>
#include <entt/entt.hpp>

#include "compTypes.hpp"

using namespace entt;
using namespace rv;

class TwoCompSimSystem : public BaseSystem<CompA, CompB>
{
	inline void update(double dt, int size, CompA* const compA, CompB* const compB) final
	{
		for (int i = 0; i < size; i++)
		{
			compA[i].x += dt;
			compA[i].y += dt;
			compB[i].x += dt;
			compB[i].y += dt;
		}
	}
};