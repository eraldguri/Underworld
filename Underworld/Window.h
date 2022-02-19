#pragma once
#include "Win32Settings.h"
#include "UnderworldException.h"
#include "Keyboard.h"

#include <optional>
#include <string>

class Window
{
private:
public:
	class Exception : public UnderWorldException
	{
	public:
		Exception(int line, const char* file, HRESULT hResult) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT m_hResult;
	};
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* m_windowClassName = "UnderworldEngine";
		static WindowClass m_windowClass;
		HINSTANCE m_hInstance;
		~WindowClass();
	};
public:
	Window(int width, int height, const std::string name) noexcept;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
	static std::optional<int> ProcessMessages();
private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessages(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard keyboard;

private:
	int m_width;
	int m_height;
	HWND m_hWnd;
};

#define CHWND_EXCEPT(m_hResult) Window::Exception(__LINE__, __FILE__, m_hResult)