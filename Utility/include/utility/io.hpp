
#pragma once

#include <string>
#include <memory>

namespace io
{
	std::unique_ptr<std::string> read(char const *filename);
	void write(char const *filename, char const *text);
}
