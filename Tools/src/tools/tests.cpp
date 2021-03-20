
#include "tools/tests.hpp"

#include "generate/noise.hpp"
#include "utility/io.hpp"

#include <thread>
#include <random>
#include <fstream>
#include <iostream>
#include <list>

// Random initialisation
static std::random_device device; // Get true random from device entropy pool
static std::mt19937 twister(device()); // Use to seed Mersenne Twister pseudorandom generator
static std::uniform_real_distribution<float> uniform(0, 10000); // Sample 0 - 10,000

void dumpFractalDistributionThread(int octaves, int samples, int bars, int *occurrences, int offset, int stride)
{
	// Run tests on allocation
	for (int i=offset; i<samples; i+=stride)
	{
		float value = ( noise::fractalSample(uniform(twister), uniform(twister), uniform(twister), 1, octaves) + 1) / 2;
		int bar = (int)floor(value*bars);
		occurrences[bar]++;
	}
}

void tests::dumpFractalDistribution(char const *filename, int octaves, int samples, int threads, int bars)
{
	// Initialise list
	int *occurrences = new int[bars];
	memset(occurrences, 0, bars*sizeof(int));

	// Run tests
	std::list<std::thread> threadList;
	for (int i=0; i<threads; i++)
	{
		threadList.push_back(std::thread(dumpFractalDistributionThread, octaves, samples, bars, occurrences, i, threads));
	}
	for (std::thread &thread : threadList)
	{
		thread.join();
	}

	// Dump values
	std::ofstream os;
	os.open(filename);
	for (int i=0; i<bars; i++)
	{
		os << occurrences[i] << std::endl;
	}
	os.close();

	// Delete array
	delete[] occurrences;
}
