
#include "utility\debug.h"

#include <exception>
#include <iostream>

void debug::zeroThrow(int value, const char* message)
{
	if (value == 0)
	{
		throw std::exception(message);
	}
}

void debug::nullThrow(void *ptr, const char* message)
{
	if (ptr == nullptr)
	{
		throw std::exception(message);
	}
}
