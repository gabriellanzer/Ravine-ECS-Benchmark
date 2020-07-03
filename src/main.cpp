#define _CRT_SECURE_NO_WARNINGS

#include <ravine/ecs.h>
#include <entt/entt.hpp>

#define ENTITIES_COUNT { 100'000, 10'000, 1'000, 1 }
#define ITERATIONS_COUNT 10000

#include "ibenchmark.h"
#include "ravineBench.hpp"
#include "enttBench.hpp"

using namespace rv;
using namespace entt;

int main(int argc, const char** argV)
{
	RavineBench ravineBench;
	ravineBench.run();

	EnttBench enttBench;
	enttBench.run();

	fprintf(stdout, "Press any key to exit.");
	getwchar();
	return 0;
}