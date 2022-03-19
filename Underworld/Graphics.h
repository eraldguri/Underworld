#pragma once

#include "Win32Settings.h"
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class Graphics
{
public:
	Graphics();
	~Graphics();

	void InitGdiPlus();

	void DrawRectangle(HDC hDeviceContext, int x, int y, int width, int height, Gdiplus::Color color);

private:
	ULONG_PTR m_gdiplusToken;
};