#pragma once
#include "Win32Settings.h"
#include <string>

class CPanel
{
public:
	CPanel(HWND hParent, HINSTANCE hInstance, int left, int right, int bottom, int top);
	~CPanel();

private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hwnd = nullptr;
};