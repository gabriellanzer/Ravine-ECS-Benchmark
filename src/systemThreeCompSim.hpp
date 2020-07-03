#pragma once
// THIS SYSTEM UPDATES 3 COMPONENT TYPES SIMULTANEOUSLY

#include <ravine/ecs.h>
#include <entt/entt.hpp>

#include "compTypes.hpp"

using namespace entt;
using namespace rv;

class ThreeCompSystem : public BaseSystem<CompA, CompB, CompC>
{
	inline void update(double dt, int size, CompA* const compA, CompB* const compB, CompC* const compC) final
	{
		for (int i = 0; i < size; i++)
		{
			compA[i].x += dt;
			compA[i].y += dt;
			compB[i].x += dt;
			compB[i].y += dt;
			compC[i].x += dt;
			compC[i].y += dt;
		}
	}
};