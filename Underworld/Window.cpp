#include "Window.h"
#include <sstream>
#include <Prsht.h>
#include <CommCtrl.h>
#pragma comment (lib, "comctl32")
#include "dwmapi.h"
#pragma comment(lib, "dwmapi")

#include "resource.h"

// Enable visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Graphics.h"
#include "Panel.h";

Window::WindowClass Window::WindowClass::m_windowClass;

Window::WindowClass::WindowClass() noexcept : m_hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = HandleMessageSetup;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetInstance();
	WindowClass.hIcon = nullptr;
	WindowClass.hCursor = nullptr;
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
	WindowClass.lpszClassName = GetName();
	WindowClass.hIconSm = nullptr;
	RegisterClassEx(&WindowClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(m_windowClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return m_windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return m_windowClass.m_hInstance;
}

Window::Window(int width, int height, const std::string name) noexcept
{
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	m_hWnd = CreateWindowEx(0, WindowClass::GetName(), name.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

std::optional<int> Window::ProcessMessages()
{
	MSG Message;
	while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		if (Message.message == WM_QUIT)
		{
			return Message.wParam;
		}

		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return {};
}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	if (uMessage == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreateStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* pWindow = static_cast<Window*>(pCreateStruct->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
		return pWindow->HandleMessages(hWnd, uMessage, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

LRESULT Window::HandleMessageThunk(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWindow->HandleMessages(hWnd, uMessage, wParam, lParam);
}

LRESULT Window::HandleMessages(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) noexcept
{
	Graphics graphics;

	switch (uMessage)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		} break;

		case WM_CREATE:
		{
			
		} break;

		case WM_PAINT:
		{
			RECT rect;
			int windowWidth = 0;
			int windowHeight = 0;
			//HRESULT correctRect = DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
			if (GetWindowRect(hWnd, &rect))
			{
				windowWidth = rect.right - rect.left;
				windowHeight = rect.bottom - rect.top;
			}

			CPanel* panel = new CPanel(hWnd, GetModuleHandle(NULL), 0, 0, windowWidth, 50);
		} break;

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_MYCOMPUTER_SYSTEMINFORMATION:
				{
				} break;

				case ID_TASKS_NEWTASK:
				{
					
				} break;
			}

		} break;

		// clear keystate when window loses focus to prevent input getting
		case WM_KILLFOCUS:
		{
			keyboard.ClearState();
		} break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) {
				keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
		} break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		} break;

		case WM_CHAR:
		{
			keyboard.OnChar(static_cast<unsigned char>(wParam));
		} break;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

Window::Exception::Exception(int line, const char* file, HRESULT hResult) noexcept
	: UnderWorldException(line, file), m_hResult(hResult)
{

}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Underworld Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hResult) noexcept
{
	char* pMessageBuffer = nullptr;
	DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer), 0, nullptr
	);
	if (nMessageLength == 0)
	{
		return "Undentified Error code";
	}
	std::string errorString = pMessageBuffer;
	LocalFree(pMessageBuffer);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return m_hResult;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_hResult);
}
