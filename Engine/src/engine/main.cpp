
#include "engine\main.h"

#include "render\display.h"

#include <iostream>
#include <exception>

int main()
{
	try
	{
		display::create(1000, 600, "Redshift");
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
