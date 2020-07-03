#pragma once
// THIS SYSTEM UPDATES 1 COMPONENT TYPE

#include <ravine/ecs.h>
#include <entt/entt.hpp>

#include "compTypes.hpp"

using namespace entt;
using namespace rv;

class OneCompSystem : public BaseSystem<CompA>
{
	inline void update(double dt, int size, CompA* const compA) final
	{
		for (int i = 0; i < size; i++)
		{
			compA[i].x += dt;
			compA[i].y += dt;
		}
	}
};