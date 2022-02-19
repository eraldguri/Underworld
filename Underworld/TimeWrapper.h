#pragma once

#include <chrono>

class TimeWrapper
{
public:
	TimeWrapper();
	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;
};