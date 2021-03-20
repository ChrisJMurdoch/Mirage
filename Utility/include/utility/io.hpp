
#pragma once

#include <string>

namespace io
{
	std::string *read(const char *filename);
	void write(char const *filename, char const *text);
}
