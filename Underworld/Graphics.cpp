#include "Graphics.h"

Graphics::Graphics()
{
	m_gdiplusToken = 0;
}

Graphics::~Graphics()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

void Graphics::InitGdiPlus()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
}

void Graphics::DrawRectangle(HDC hDeviceContext, int x, int y, int width, int height, Gdiplus::Color color)
{
	Gdiplus::SolidBrush brush(color);

	Gdiplus::Graphics gfx(hDeviceContext);

	gfx.FillRectangle(&brush, x, y, width, height);
}
