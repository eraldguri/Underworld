#include "App.h"

#include <sstream>
#include <iomanip>

App::App() : window(1200, 600, "UNDERWORLD")
{
	
}

int App::Run()
{
	MSG Message;
	BOOL gResult;
	while ((gResult = GetMessage(&Message, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);

		DoFrame();
	}

	return Message.wParam;
}

void App::DoFrame()
{
}
