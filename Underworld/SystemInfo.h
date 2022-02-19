#pragma once

#include "Win32Settings.h"
#include <CommCtrl.h>
#include <string>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <shlwapi.h>

constexpr int NUM_PAGES = 2;

class SystemInfo
{
public:
	static char* GetComputerDetails();

public:
	static VOID DoPropertySheet(HWND hwndOwner);

private:
	static int InsertListViewColumns(HWND hListView, long lCol, int iPercent, char* text);
	static int InsertListViewItems(HWND hListView, long lLin, long lCol, int iSubItem, char* text);

	static LRESULT CALLBACK SummaryProc(HWND hwndDialog, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Tab2Proc(HWND hwndDialog, UINT uMessage, WPARAM wParam, LPARAM lParam);

	static bool GetWinMajorMinorVersion(DWORD& major, DWORD& minor);
	static const std::string GetWindowVersionString();
	static UINT GetComputerManufacturer(LPSTR lpBuffer, UINT uSize);

	static BOOL Is64BitWindows();
	static DWORDLONG  GetTotalPhysicalMemory();
};

