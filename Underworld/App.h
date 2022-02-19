#pragma once

#include "Window.h"
#include "TimeWrapper.h"

class App
{
public:
	App();

	int Run();

private:
	void DoFrame();

private:
	Window window;
	TimeWrapper timer;
};