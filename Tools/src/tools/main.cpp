
#include "tools/main.h"

#include "tools/tests.h"

#include <exception>
#include <iostream>

int main()
{
	try
	{
		int const SAMPLES = 100000000, THREADS = 16;
		int octaves;

		octaves = 1;
		std::cout << "Dumping fractal distribution for " << octaves << " octaves." << std::endl;
		tests::dumpFractalDistribution("ToolData/AdjustedFractalDistribution1.txt", octaves, true, SAMPLES, THREADS);

		octaves = 2;
		std::cout << "Dumping fractal distribution for " << octaves << " octaves." << std::endl;
		tests::dumpFractalDistribution("ToolData/AdjustedFractalDistribution2.txt", octaves, true, SAMPLES, THREADS);

		octaves = 3;
		std::cout << "Dumping fractal distribution for " << octaves << " octaves." << std::endl;
		tests::dumpFractalDistribution("ToolData/AdjustedFractalDistribution3.txt", octaves, true, SAMPLES, THREADS);

		octaves = 4;
		std::cout << "Dumping fractal distribution for " << octaves << " octaves." << std::endl;
		tests::dumpFractalDistribution("ToolData/AdjustedFractalDistribution4.txt", octaves, true, SAMPLES, THREADS);

		octaves = 5;
		std::cout << "Dumping fractal distribution for " << octaves << " octaves." << std::endl;
		tests::dumpFractalDistribution("ToolData/AdjustedFractalDistribution5.txt", octaves, true, SAMPLES, THREADS);

		std::cout << "Done." << std::endl;
		return 0;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
}
