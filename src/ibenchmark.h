#pragma once

// Default Values for Benchmark
#ifndef ENTITIES_COUNT
#define ENTITIES_COUNT { 100'000 }
#endif
#ifndef ITERATIONS_COUNT
#define ITERATIONS_COUNT 10'000
#endif

#include <stdio.h>
#include <string.h>
#include <chrono>
#include <time.h>

using std::string;
using std::to_string;
using std::chrono::duration;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

static const int entitiesCount[] = ENTITIES_COUNT;

/// <summary>
/// Little trick to get size of constant arrays.
/// </summary>
template<typename T, int sz>
static int size(T(&)[sz])
{
	return sz;
}

/// <summary>
/// Benchmark Interface for all the benchmarks.
/// </summary>
class IBenchmark
{
private:
	FILE* logFile;
	double samples[ITERATIONS_COUNT];
	double mean = FLT_MAX;
	double stddev = FLT_MAX;

public:
	/// <summary>
	/// Should return the library name used for this benchmark.
	/// </summary>
	/// <returns>Library name for this benchmark.</returns>
	inline virtual const char* getName() = 0;
	
	inline virtual void setupOneComp(int entityCount) = 0;
	inline virtual void setupTwoCompSep(int entityCount) = 0;
	inline virtual void setupTwoCompSim(int entityCount) = 0;
	inline virtual void setupThreeComp(int entityCount) = 0;
	inline virtual void setupThreeCompPair(int entityCount) = 0;

	inline virtual void tickOneComp(double deltaTime) = 0;
	inline virtual void tickTwoCompSep(double deltaTime) = 0;
	inline virtual void tickTwoCompSim(double deltaTime) = 0;
	inline virtual void tickThreeComp(double deltaTime) = 0;
	inline virtual void tickThreeCompPair(double deltaTime) = 0;

	/// <summary>
	/// Should be used for cleanup of the benchmark test data.
	/// </summary>
	inline virtual void cleanup() = 0;

	/// <summary>
	/// Actually runs the benchmark, from start to end.
	/// </summary>
	inline void run()
	{
		// Get Log Prefix
		string benchName = getName();
		time_t t = time(NULL);
		struct tm date = *localtime(&t);
		string logPrefix = (benchName + " (" + to_string(date.tm_year + 1900) + "-" + to_string(date.tm_mon + 1)
			+ "-" + to_string(date.tm_mday) + " " + to_string(date.tm_hour) + "h" + to_string(date.tm_min)
			+ "m" + to_string(date.tm_sec)) + "s)";

		fprintf(stdout, "\n\n::Starting benchmark for %s::\n", benchName.c_str());

#pragma region One Comp Benchmark
		fprintf(stdout, "\nStarting Test - *One Component*\n");
		// Open Log File
		string logName = logPrefix + " - One Component.csv";
		fprintf(stdout, "Writting log to '%s'.\n", logName.c_str());
		logFile = fopen(logName.c_str(), "w");
		if (logFile == NULL)
		{
			fprintf(stderr, "Unable to create log file!\n");
			exit(1);
		}
		for (int entCount : entitiesCount)
		{
			fprintf(stdout, "Testing with %i entities\n", entCount);
			fprintf(logFile, "%i Entities;", entCount);

			// Setup Benchmark
			fprintf(stdout, "Allocating Entities... ");
			setupOneComp(entCount);
			fprintf(stdout, "Done! ");

			// Perform benchmark with the given iterations count
			fprintf(stdout, "Performing tick simulations... ");
			double acc = 0;
			double deltaTime = 0.016;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				auto start = high_resolution_clock::now();

				tickOneComp(deltaTime);

				auto end = high_resolution_clock::now();
				auto elapsed = duration_cast<nanoseconds>(end - start);
				deltaTime = elapsed.count() / 1'000'000.0;
				samples[i] = deltaTime;
				acc += deltaTime;
			}
			fprintf(stdout, "Done! ");

			// Calculate mean and std deviation values
			mean = acc / (double)ITERATIONS_COUNT;
			double var = 0;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				fprintf(logFile, "%.7f;", samples[i]);
				var += (samples[i] - mean) * (samples[i] - mean);
			}
			fprintf(logFile, "\n");
			var /= (double)ITERATIONS_COUNT;
			stddev = sqrt(var);

			// Cleanup
			fprintf(stdout, "Performing clean-up... ");
			cleanup();
			fprintf(stdout, "Done!\n");

			// Log
			fprintf(stdout, "Finished with mean time %.7fms, and std-dev %.7fms!\n", mean, stddev);
		}
		fclose(logFile);
#pragma endregion

#pragma region Two Comp Separately Benchmark
		fprintf(stdout, "\nStarting Test - *Two Components Separately*\n");
		// Open Log File
		logName = logPrefix + " - Two Components Separately.csv";
		fprintf(stdout, "Writting log to '%s'.\n", logName.c_str());
		logFile = fopen(logName.c_str(), "w");
		if (logFile == NULL)
		{
			fprintf(stderr, "Unable to create log file!\n");
			exit(1);
		}
		for (int entCount : entitiesCount)
		{
			fprintf(stdout, "Testing with %i entities\n", entCount);
			fprintf(logFile, "%i Entities;", entCount);

			// Setup Benchmark
			fprintf(stdout, "Allocating Entities... ");
			setupTwoCompSep(entCount);
			fprintf(stdout, "Done! ");

			// Perform benchmark with the given iterations count
			fprintf(stdout, "Performing tick simulations... ");
			double acc = 0;
			double deltaTime = 0.016;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				auto start = high_resolution_clock::now();

				tickTwoCompSep(deltaTime);

				auto end = high_resolution_clock::now();
				auto elapsed = duration_cast<nanoseconds>(end - start);
				deltaTime = elapsed.count() / 1'000'000.0;
				samples[i] = deltaTime;
				acc += deltaTime;
			}
			fprintf(stdout, "Done! ");

			// Calculate mean and std deviation values
			mean = acc / (double)ITERATIONS_COUNT;
			double var = 0;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				fprintf(logFile, "%.7f;", samples[i]);
				var += (samples[i] - mean) * (samples[i] - mean);
			}
			fprintf(logFile, "\n");
			var /= (double)ITERATIONS_COUNT;
			stddev = sqrt(var);

			// Cleanup
			fprintf(stdout, "Performing clean-up... ");
			cleanup();
			fprintf(stdout, "Done!\n");

			// Log
			fprintf(stdout, "Finished with mean time %.7fms, and std-dev %.7fms!\n", mean, stddev);
		}
		fclose(logFile);
#pragma endregion

#pragma region Two Comp Simultaneously Benchmark
		fprintf(stdout, "\nStarting Test - *Two Components Simultaneously*\n");
		// Open Log File
		logName = logPrefix + " - Two Components Simultaneously.csv";
		fprintf(stdout, "Writting log to '%s'.\n", logName.c_str());
		logFile = fopen(logName.c_str(), "w");
		if (logFile == NULL)
		{
			fprintf(stderr, "Unable to create log file!\n");
			exit(1);
		}
		for (int entCount : entitiesCount)
		{
			fprintf(stdout, "Testing with %i entities\n", entCount);
			fprintf(logFile, "%i Entities;", entCount);

			// Setup Benchmark
			fprintf(stdout, "Allocating Entities... ");
			setupTwoCompSim(entCount);
			fprintf(stdout, "Done! ");

			// Perform benchmark with the given iterations count
			fprintf(stdout, "Performing tick simulations... ");
			double acc = 0;
			double deltaTime = 0.016;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				auto start = high_resolution_clock::now();

				tickTwoCompSim(deltaTime);

				auto end = high_resolution_clock::now();
				auto elapsed = duration_cast<nanoseconds>(end - start);
				deltaTime = elapsed.count() / 1'000'000.0;
				samples[i] = deltaTime;
				acc += deltaTime;
			}
			fprintf(stdout, "Done! ");

			// Calculate mean and std deviation values
			mean = acc / (double)ITERATIONS_COUNT;
			double var = 0;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				fprintf(logFile, "%.7f;", samples[i]);
				var += (samples[i] - mean) * (samples[i] - mean);
			}
			fprintf(logFile, "\n");
			var /= (double)ITERATIONS_COUNT;
			stddev = sqrt(var);

			// Cleanup
			fprintf(stdout, "Performing clean-up... ");
			cleanup();
			fprintf(stdout, "Done!\n");

			// Log
			fprintf(stdout, "Finished with mean time %.7fms, and std-dev %.7fms!\n", mean, stddev);
		}
		fclose(logFile);
#pragma endregion

#pragma region Three Comp by Pairs Benchmark
		fprintf(stdout, "\nStarting Test - *Three Components by Pairs*\n");
		// Open Log File
		logName = logPrefix + " - Three Components by Pairs.csv";
		fprintf(stdout, "Writting log to '%s'.\n", logName.c_str());
		logFile = fopen(logName.c_str(), "w");
		if (logFile == NULL)
		{
			fprintf(stderr, "Unable to create log file!\n");
			exit(1);
		}
		for (int entCount : entitiesCount)
		{
			fprintf(stdout, "Testing with %i entities\n", entCount);
			fprintf(logFile, "%i Entities;", entCount);

			// Setup Benchmark
			fprintf(stdout, "Allocating Entities... ");
			setupThreeCompPair(entCount);
			fprintf(stdout, "Done! ");

			// Perform benchmark with the given iterations count
			fprintf(stdout, "Performing tick simulations... ");
			double acc = 0;
			double deltaTime = 0.016;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				auto start = high_resolution_clock::now();

				tickThreeCompPair(deltaTime);

				auto end = high_resolution_clock::now();
				auto elapsed = duration_cast<nanoseconds>(end - start);
				deltaTime = elapsed.count() / 1'000'000.0;
				samples[i] = deltaTime;
				acc += deltaTime;
			}
			fprintf(stdout, "Done! ");

			// Calculate mean and std deviation values
			mean = acc / (double)ITERATIONS_COUNT;
			double var = 0;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				fprintf(logFile, "%.7f;", samples[i]);
				var += (samples[i] - mean) * (samples[i] - mean);
			}
			fprintf(logFile, "\n");
			var /= (double)ITERATIONS_COUNT;
			stddev = sqrt(var);

			// Cleanup
			fprintf(stdout, "Performing clean-up... ");
			cleanup();
			fprintf(stdout, "Done!\n");

			// Log
			fprintf(stdout, "Finished with mean time %.7fms, and std-dev %.7fms!\n", mean, stddev);
		}
		fclose(logFile);
#pragma endregion

#pragma region Three Comp Simultaneously Benchmark
		fprintf(stdout, "\nStarting Test - *Three Components Simultaneously*\n");
		// Open Log File
		logName = logPrefix + " - Three Components Simultaneously.csv";
		fprintf(stdout, "Writting log to '%s'.\n", logName.c_str());
		logFile = fopen(logName.c_str(), "w");
		if (logFile == NULL)
		{
			fprintf(stderr, "Unable to create log file!\n");
			exit(1);
		}
		for (int entCount : entitiesCount)
		{
			fprintf(stdout, "Testing with %i entities\n", entCount);
			fprintf(logFile, "%i Entities;", entCount);

			// Setup Benchmark
			fprintf(stdout, "Allocating Entities... ");
			setupThreeComp(entCount);
			fprintf(stdout, "Done! ");

			// Perform benchmark with the given iterations count
			fprintf(stdout, "Performing tick simulations... ");
			double acc = 0;
			double deltaTime = 0.016;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				auto start = high_resolution_clock::now();

				tickThreeComp(deltaTime);

				auto end = high_resolution_clock::now();
				auto elapsed = duration_cast<nanoseconds>(end - start);
				deltaTime = elapsed.count() / 1'000'000.0;
				samples[i] = deltaTime;
				acc += deltaTime;
			}
			fprintf(stdout, "Done! ");

			// Calculate mean and std deviation values
			mean = acc / (double)ITERATIONS_COUNT;
			double var = 0;
			for (size_t i = 0; i < ITERATIONS_COUNT; i++)
			{
				fprintf(logFile, "%.7f;", samples[i]);
				var += (samples[i] - mean) * (samples[i] - mean);
			}
			fprintf(logFile, "\n");
			var /= (double)ITERATIONS_COUNT;
			stddev = sqrt(var);

			// Cleanup
			fprintf(stdout, "Performing clean-up... ");
			cleanup();
			fprintf(stdout, "Done!\n");

			// Log
			fprintf(stdout, "Finished with mean time %.7fms, and std-dev %.7fms!\n", mean, stddev);
		}
		fclose(logFile);
#pragma endregion


		// Finished
		fprintf(stdout, "\n::Benchmark for %s complete::\n", benchName.c_str());
	}
};
