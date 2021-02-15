#pragma once

namespace debug
{
	void zeroThrow(int value, const char* message="Zero error");
	void nullThrow(void *ptr, const char* message="Null error");
}
