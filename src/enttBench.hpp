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

using namespace rv;
using entt::basic_group;
using entt::entity;
using entt::exclude_t;
using entt::get_t;

typedef basic_group<entity, exclude_t<>, get_t<>, CompA, CompB> groupAB;
typedef basic_group<entity, exclude_t<>, get_t<>, CompB, CompC> groupBC;
typedef basic_group<entity, exclude_t<>, get_t<>, CompA, CompB, CompC> groupABC;

class EnttBench : public IBenchmark
{
private:
	registry reg;
	groupAB gab = reg.group<CompA, CompB>();
	groupBC gbc = reg.group<CompB, CompC>();
	groupABC gabc = reg.group<CompA, CompB, CompC>();


public:
	inline const char* getName() final
	{
		return "Entt";
	}

	inline void setupOneComp(int entityCount) final
	{
		for (int i = 0; i < entityCount; i++)
		{
			auto entity = reg.create();
			reg.emplace<CompA>(entity);
		}
	}
	inline void setupTwoCompSim(int entityCount) final
	{
		for (int i = 0; i < entityCount; i++)
		{
			auto entity = reg.create();
			reg.emplace<CompA>(entity);
			reg.emplace<CompB>(entity);
		}
	}
	inline void setupTwoCompSep(int entityCount) final
	{
		for (int i = 0; i < entityCount; i++)
		{
			auto entity = reg.create();
			reg.emplace<CompA>(entity);
			reg.emplace<CompB>(entity);
		}
	}
	inline void setupThreeComp(int entityCount) final
	{
		for (int i = 0; i < entityCount; i++)
		{
			auto entity = reg.create();
			reg.emplace<CompA>(entity);
			reg.emplace<CompB>(entity);
			reg.emplace<CompC>(entity);
		}
	}
	inline void setupThreeCompPair(int entityCount) final
	{

		for (int i = 0; i < entityCount; i++)
		{
			auto entity = reg.create();
			reg.emplace<CompA>(entity);
			reg.emplace<CompB>(entity);
			reg.emplace<CompC>(entity);
		}
	}

	inline void tickOneComp(double dt) final
	{
		auto view = reg.view<CompA>();
		int size = view.size();
		CompA* compA = view.raw();
		for (int i = 0; i < size; i++)
		{
			compA[i].x += dt;
			compA[i].y += dt;
		}
	}
	inline void tickTwoCompSim(double dt) final
	{
		gab.each(
			[dt](CompA& compA, CompB& compB)
			{
				compA.x += dt;
				compA.y += dt;
				compB.x += dt;
				compB.y += dt;
			}
		);
	}
	inline void tickTwoCompSep(double dt) final
	{
		for (auto entity : gab) {
			auto& compA = gab.get<CompA>(entity);
			compA.x += dt;
			compA.y += dt;
		}
		for (auto entity : gab) {
			auto& compB = gab.get<CompB>(entity);
			compB.x += dt;
			compB.y += dt;
		}
	}
	inline void tickThreeComp(double dt) final
	{
		gabc.each(
			[dt](CompA& compA, CompB& compB, CompC& compC)
			{
				compA.x += dt;
				compA.y += dt;
				compB.x += dt;
				compB.y += dt;
				compC.x += dt;
				compC.y += dt;
			}
		);
	}
	inline void tickThreeCompPair(double dt) final
	{
		gab.each(
			[dt](CompA& compA, CompB& compB)
			{
				compA.x += dt;
				compA.y += dt;
				compB.x += dt;
				compB.y += dt;
			}
		);

		gbc.each(
			[dt](CompB& compB, CompC& compC)
			{
				compB.x += dt;
				compB.y += dt;
				compC.x += dt;
				compC.y += dt;
			}
		);
	}

	inline void cleanup() final
	{
		reg.clear();
	}
};