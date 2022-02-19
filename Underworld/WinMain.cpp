#include "Window.h"
#include "App.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	try {
		return App{}.Run();
	}
	catch (const UnderWorldException& exp)
	{
		MessageBoxA(nullptr, exp.what(), exp.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& exp)
	{
		MessageBoxA(nullptr, exp.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}