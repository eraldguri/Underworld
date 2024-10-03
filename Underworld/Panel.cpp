#include "Panel.h"

std::string ClassName = "PanelClassName";
std::string ControlName = "Panel";

CPanel::CPanel(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height)
{
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = HandleMessageSetup;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInstance;
	WindowClass.hIcon = nullptr;
	WindowClass.hCursor = nullptr;
	WindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 222, 173));
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = ClassName.c_str();
	WindowClass.hIconSm = nullptr;
	RegisterClassEx(&WindowClass);

	m_hwnd = CreateWindowEx(0, ClassName.c_str(), ControlName.c_str(), WS_CHILD | WS_VISIBLE,
		x, y, width, height,
		hParent, (HMENU)1234, hInstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

CPanel::~CPanel()
{
}

LRESULT CPanel::HandleMessageSetup(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE: {} break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		int width = 0;
		int height = 0;
		RECT rect;
		if (GetWindowRect(hWnd, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}

		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 10, 10, TEXT("GRADECI"), strlen("GRADECI"));

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
	} break;
	case WM_CLOSE: 
	{
		DestroyWindow(hWnd);
	} break;
	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
}
