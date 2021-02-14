
#include "engine\main.h"

#include "render\display.h"

#include <iostream>
#include <exception>

int main()
{
	std::cout << "Starting..." << std::endl;
	try
	{
		display::create(1000, 600, "Redshift");
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Finished." << std::endl;
	return 0;
}
