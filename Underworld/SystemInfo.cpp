#include "SystemInfo.h"
#include <wchar.h>
#include <string>
#include "resource.h"
#include "MemoryInfo.h"

constexpr int MAX_NUM_THREADS = 8;
constexpr int MAX_CPU_USAGE_PERCENT = 100;
constexpr int BUFFER_SIZE = 255;

HANDLE hLoadingThreads[MAX_NUM_THREADS];
HANDLE hWatchingThreads = INVALID_HANDLE_VALUE;
ULONGLONG ullCounter = UINT_MAX;

char* SystemInfo::GetComputerDetails()
{
	char ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(ComputerName) / sizeof(ComputerName[0]);

    BOOL hResult = GetComputerName(ComputerName, &size);
	if (hResult == NULL)
	{
		wprintf(L"Failed to get computer name %ld", GetLastError());
	}

    return ComputerName;
}

VOID SystemInfo::DoPropertySheet(HWND hwndOwner)
{
    PROPSHEETPAGE psp[2];

    PROPSHEETHEADER psh;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[0].hInstance = GetModuleHandle(NULL);
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_FORMVIEW1);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = SummaryProc;
    psp[0].pszTitle = const_cast<LPTSTR>(TEXT("System Summary"));
    psp[0].lParam = 0;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[1].hInstance = GetModuleHandle(NULL);
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_FORMVIEW2);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = Tab2Proc;
    psp[1].pszTitle = const_cast<LPTSTR>(TEXT("Tab 2"));
    psp[1].lParam = 0;
    psp[1].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
    psh.hwndParent = hwndOwner;
    psh.hInstance = GetModuleHandle(NULL);
    psh.pszIcon = MAKEINTRESOURCE(IDD_SYSTEM_INFO_DIALOG);
    psh.pszCaption = const_cast<LPTSTR>(TEXT("System Info"));
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = (LPCPROPSHEETPAGE)&psp;
    psh.pfnCallback = NULL;

    PropertySheet(&psh);

    return;
}

int SystemInfo::InsertListViewColumns(HWND hListView, long lCol, int iPercent, char* text)
{
    LVCOLUMN Column;
    RECT rect;
    GetClientRect(hListView, &rect);

    static const char* sApplication = "Underworld";
    char sErrorString[5000];

    int iResult;

    iPercent = iPercent > 10 ? min(iPercent, 90) : 10;
    int iWidth = (int)(rect.right * (iPercent / 100.0));

    if (hListView == NULL)
    {
        sprintf_s(sErrorString,
            "! Handle of ListView NULL (fInsertListViewColumn)");
        MessageBox(NULL, (LPSTR)sErrorString, (LPSTR)sApplication, MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    memset(&Column, 0, sizeof(Column));
    Column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
    Column.fmt = LVCFMT_LEFT;
    Column.pszText = text;
    Column.iSubItem = lCol;
    Column.cx = iWidth;

    if (SendMessage(hListView, (UINT)LVM_INSERTCOLUMN, (WPARAM)(int)lCol, (LPARAM)&Column) == -1) {
        iResult = 0;
    }
    else {
        iResult = 1;
    }

    InvalidateRect(hListView, &rect, TRUE);

    return iResult;
}

int SystemInfo::InsertListViewItems(HWND hListView, long lLin, long lCol, int iSubItem, char* text)
{
    LVITEM Item;
    RECT rect;

    GetClientRect(hListView, &rect);

    static const char* sApplication = "Underworld";
    char sErrorString[5000];

    int iResult;

    if (hListView == NULL)
    {
        sprintf_s(sErrorString,
            "! Handle of ListView NULL (fInsertListViewItem)");
        MessageBox(NULL, (LPSTR)sErrorString, (LPSTR)sApplication, MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    memset(&Item, 0, sizeof(Item));
    Item.mask = LVIF_TEXT;
    Item.state = 0;
    Item.stateMask = 0;
    Item.pszText = text;
    Item.iItem = lLin;
    Item.iSubItem = lCol;

    switch (iSubItem)
    {
        case 0:
        {
            if (SendMessage(hListView, (UINT)LVM_INSERTITEM, (WPARAM)0, (LPARAM)&Item) == -1)
            {
                iResult = 0;
            }
            else
            {
                iResult = 1;
            }
        } break;

        case 1:
        {
            if (SendMessage(hListView, (UINT)LVM_SETITEM, (WPARAM)0, (LPARAM)&Item) == FALSE)
            {
                iResult = 0;
            }
            else
            {
                iResult = 1;
            }
        } break;

        default:
        {
            sprintf_s(sErrorString, "! Unexpected iSubItemYesNo value: %d (fInsertListViewItem)", iSubItem);
            MessageBox(NULL, (LPSTR)sErrorString, (LPSTR)sApplication, MB_OK | MB_ICONEXCLAMATION);
            return 0;
        } break;
    }

    InvalidateRect(hListView, &rect, TRUE);

    return iResult;
}

LRESULT SystemInfo::SummaryProc(HWND hwndDialog, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch (uMessage)
    {
        case WM_INITDIALOG:
        {
            RECT rect = { 0 };
            GetClientRect(hwndDialog, &rect);

            HWND hListView = CreateWindowEx(0,
                WC_LISTVIEW,
                NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_SHOWSELALWAYS | LVS_REPORT,
                0, 0, rect.right - rect.left, rect.bottom - rect.top, hwndDialog, NULL, NULL, NULL);
           
            int iColumnsToInsert = 2;
            int iLinesToInsert = 1;
          
            char* windowsVersion = new char[GetWindowVersionString().length() + 1];
            std::strcpy(windowsVersion, GetWindowVersionString().c_str());

            TCHAR szBuffer[128];
            szBuffer[0] = 0;

            if (GetComputerManufacturer(szBuffer, 128) < 0)
            {
                wsprintf(szBuffer, "Failed: %d", GetLastError());
            }

            char* kernelVersion = new char[GetNtDllVersion().length() + 1];
            std::strcpy(kernelVersion, GetNtDllVersion().c_str());

            char* language = new char[GetLanguage().length() + 1];
            std::strcpy(language, GetLanguage().c_str());

            MemoryInfo* memoryInfo = new MemoryInfo();
            char* totalRam = new char[memoryInfo->GetTotalRam().length() + 1];
            std::strcpy(totalRam, memoryInfo->GetTotalRam().c_str());

            char* availableRam = new char[memoryInfo->GetAvailableRam().length() + 1];
            std::strcpy(availableRam, memoryInfo->GetAvailableRam().c_str());

            char* totalPageFile = new char[memoryInfo->GetTotalPageFile().length() + 1];
            std::strcpy(totalPageFile, memoryInfo->GetTotalPageFile().c_str());

            char* availablePageFile = new char[memoryInfo->GetAvailablePageFile().length() + 1];
            std::strcpy(availablePageFile, memoryInfo->GetAvailablePageFile().c_str());

            char* totalVirtualMemory = new char[memoryInfo->GetTotalVirtualMemory().length() + 1];
            std::strcpy(totalVirtualMemory, memoryInfo->GetTotalVirtualMemory().c_str());

            char* availableVirtualMemory = new char[memoryInfo->GetAvailableVirtualMemory().length() + 1];
            std::strcpy(availableVirtualMemory, memoryInfo->GetAvailableVirtualMemory().c_str());

            const char* systemType;
            if (Is64BitWindows())
            {
                systemType = "x64-based PC";
            }
            else
            {
                systemType = "x86-based PC";
            }

            const char* sListColumnText[2]          = {"Item", "Value"};
            const char* sFirstRow[2]                = { "OS Name ", windowsVersion };
            const char* sSecondRow[2]               = { "Computer Name", GetComputerDetails()};
            const char* thirdRow[2]                 = { "OS Manufacturer", szBuffer };
            const char* fourthRow[2]                = { "System Type", systemType };
            const char* sixthRow[2]                 = { "Kernel Version", kernelVersion };
            const char* languageRow[2]              = { "Language", language };
            const char* totalRamRow[2]              = { "Total Ram", totalRam };
            const char* availableRamRow[2]          = { "Availabe Ram", availableRam };
            const char* totalPageFileRow[2]         = { "Total Page File", totalPageFile };
            const char* availablePageFileRow[2]     = { "Available Page File", availablePageFile };
            const char* totalVirtualMemoryRow[2]    = { "Total Virtual Memory", totalVirtualMemory };
            const char* totalAvailableMemoryRow[2]  = { "Total Available Memory", availableVirtualMemory };

            for (int i = 0; i < iColumnsToInsert; ++i)
            {
                InsertListViewColumns(hListView, i, 100 / iColumnsToInsert, const_cast<char*>(sListColumnText[i]));
            }

            for (int i = 0; i < iColumnsToInsert; ++i)
            {
                for (int j = 0; j < iLinesToInsert; ++j)
                {
                    InsertListViewItems(hListView, 0, i, i == 0 ? 0 : 1, const_cast<char*>(sFirstRow[i]));
                    InsertListViewItems(hListView, 1, i, i == 0 ? 0 : 1, const_cast<char*>(sSecondRow[i]));
                    InsertListViewItems(hListView, 2, i, i == 0 ? 0 : 1, const_cast<char*>(thirdRow[i]));
                    InsertListViewItems(hListView, 3, i, i == 0 ? 0 : 1, const_cast<char*>(fourthRow[i]));
                    InsertListViewItems(hListView, 4, i, i == 0 ? 0 : 1, const_cast<char*>(sixthRow[i]));
                    InsertListViewItems(hListView, 5, i, i == 0 ? 0 : 1, const_cast<char*>(languageRow[i]));
                    InsertListViewItems(hListView, 6, i, i == 0 ? 0 : 1, const_cast<char*>(totalRamRow[i]));
                    InsertListViewItems(hListView, 7, i, i == 0 ? 0 : 1, const_cast<char*>(availableRamRow[i]));
                    InsertListViewItems(hListView, 8, i, i == 0 ? 0 : 1, const_cast<char*>(totalPageFileRow[i]));
                    InsertListViewItems(hListView, 9, i, i == 0 ? 0 : 1, const_cast<char*>(availablePageFileRow[i]));
                    InsertListViewItems(hListView, 10, i, i == 0 ? 0 : 1, const_cast<char*>(totalVirtualMemoryRow[i]));
                    InsertListViewItems(hListView, 11, i, i == 0 ? 0 : 1, const_cast<char*>(totalAvailableMemoryRow[i]));
                }
            }

        } break;
    }
   
    return DefWindowProc(hwndDialog, uMessage, wParam, lParam);
}

LRESULT SystemInfo::Tab2Proc(HWND hwndDialog, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

bool SystemInfo::GetWinMajorMinorVersion(DWORD& major, DWORD& minor)
{
    bool bReturnCode = false;
    LPBYTE pInfoRawData = 0;
    if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pInfoRawData))
    {
        WKSTA_INFO_100* pWorkstationInfo = reinterpret_cast<WKSTA_INFO_100*>(pInfoRawData);
        major = pWorkstationInfo->wki100_ver_major;
        minor = pWorkstationInfo->wki100_ver_minor;
        NetApiBufferFree(pInfoRawData);
        bReturnCode = true;
    }
    return bReturnCode;
}

const std::string SystemInfo::GetWindowVersionString()
{
    std::string windowsVersion;
    OSVERSIONINFOEX osver;
    SYSTEM_INFO SystemInfo;
    typedef void(__stdcall* GETSYSTEMINFO)(LPSYSTEM_INFO);

    __pragma(warning(push))
        __pragma(warning(disable:4996))

            memset(&osver, 0, sizeof(osver));
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx((LPOSVERSIONINFO)&osver);
    __pragma(warning(pop))
        DWORD major = 0;
    DWORD minor = 0;
    if (GetWinMajorMinorVersion(major, minor))
    {
        osver.dwMajorVersion = major;
        osver.dwMinorVersion = minor;
    }
    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
    {
        OSVERSIONINFOEXW osvi;
        ULONGLONG ConditionMask = 0;
        ConditionMask = VerSetConditionMask(ConditionMask, VER_MINORVERSION, VER_EQUAL);
        ZeroMemory(&osvi, sizeof(osvi));
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        osvi.dwMinorVersion = 3;
        if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, ConditionMask))
        {
            osver.dwMinorVersion = 3;
        }
    }

    GETSYSTEMINFO sysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandle("kernel32.dll"), "GetNativeSystemInfo");
    if (sysInfo == NULL)
    {
        sysInfo = GetSystemInfo;
    }
    sysInfo(&SystemInfo);

    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows 10 Server";
    }
    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows 10";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows Server 2012 R2";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows 8.1";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows Server 2012";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows 8";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows Server 2008 R2";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows 7";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows Server 2008";
    }
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)
    {
        windowsVersion = "Windows Vista";
    }
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION &&
        SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
    {
        windowsVersion = "Windows XP x64";
    }
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
    {
        windowsVersion = "Windows Server 2003";
    }
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
    {
        windowsVersion = "Windows XP";
    }
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
    {
        windowsVersion = "Windows 200";
    }
    if (osver.dwMajorVersion < 5)
    {
        windowsVersion = "Unknown";
    }

    if (osver.wServicePackMajor != 0)
    {
        std::string servicePack;
        char buffer[128] = { 0 };
        servicePack = " Service Pack ";
        windowsVersion += servicePack;
    }

    if (strlen(osver.szCSDVersion) > 0)
    {
        windowsVersion += " ";
        windowsVersion += osver.szCSDVersion;
    }

    windowsVersion += " (Build " + std::to_string((int)osver.dwBuildNumber) + ")";

    if (osver.dwMajorVersion >= 6)
    {
        if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        {
            windowsVersion += ", 64-bit";
        }
        else if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
        {
            windowsVersion += ", 32-bit";
        }
    }

    return windowsVersion;

}

UINT SystemInfo::GetComputerManufacturer(LPSTR lpBuffer, UINT uSize)
{
    HKEY KeyData;
    HANDLE HandleHeap = NULL;
    LPSTR lpString = NULL;
    LPBYTE lpData = NULL;
    DWORD dwType = 0, dwSize = 0;
    UINT uIndex, uStart, uEnd, uString, uLength, uState = 0;
    LONG lError;

    if ((lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Services\\mssmbios\\Data"),
        0, KEY_QUERY_VALUE, &KeyData)) != ERROR_SUCCESS) 
    {
        SetLastError(lError);
        return 0;
    }

    if ((lError = RegQueryValueEx(KeyData, TEXT("SMBiosData"), NULL, &dwType, NULL, &dwSize)) == ERROR_SUCCESS)
    {
        if (dwSize == 0 || dwType != REG_BINARY)
        {
            lError = ERROR_BADKEY;
        }
        else
        {
            HandleHeap = GetProcessHeap();
            lpData = (LPBYTE)HeapAlloc(HandleHeap, 0, dwSize);
            if (!lpData)
            {
                lError = ERROR_NOT_ENOUGH_MEMORY;
            }
            else
            {
                lError = RegQueryValueEx(KeyData, TEXT("SMBiosData"), NULL, NULL, lpData, &dwSize);
            }
        }
    }
    RegCloseKey(KeyData);

    if (lError == ERROR_SUCCESS)
    {
        uIndex = 8 + *(WORD*)(lpData + 6);
        uEnd   = 8 + *(WORD*)(lpData + 4);
        while (lpData[uIndex] != 0x7F && uIndex < uEnd)
        {
            uIndex += lpData[(uStart = uIndex) + 1];
            uString = 1;
            do {
                if (lpData[uStart] == 0x01 && uState == 0)
                {
                    if (lpData[uStart + 4] == uString || lpData[uStart + 5] == uString || lpData[uStart + 6] == uString)
                    {
                        lpString = (LPSTR)(lpData + uIndex);
                        if (!StrCmp(lpString, "System manufacturer"))
                        {
                            lpString = NULL;
                            uState++;
                        }
                    }
                }
                else if (lpData[uStart] == 0x02 && uState == 1)
                {
                    if (lpData[uStart + 4] == uString || lpData[uStart + 5] == uString || lpData[uStart + 6] == uString)
                    {
                        lpString = (LPSTR)(lpData + uIndex);
                    }
                }
                else if (lpData[uStart] == 0x03 && uString == 1)
                {
                    switch (lpData[uStart + 5])
                    {
                        default:   lpString = (LPSTR)TEXT("(Other)");               break;
                        case 0x02: lpString = (LPSTR)TEXT("(Unknown)");             break;
                        case 0x03: lpString = (LPSTR)TEXT("(Desktop)");             break;
                        case 0x04: lpString = (LPSTR)TEXT("(Low Profile Desktop)"); break;
                        case 0x06: lpString = (LPSTR)TEXT("(Mini Tower)");          break;
                        case 0x07: lpString = (LPSTR)TEXT("(Tower)");               break;
                        case 0x08: lpString = (LPSTR)TEXT("(Portable)");            break;
                        case 0x09: lpString = (LPSTR)TEXT("(Laptop)");              break;
                        case 0x0A: lpString = (LPSTR)TEXT("(Notebook)");            break;
                        case 0x0E: lpString = (LPSTR)TEXT("(Sub Notebook)");        break;
                    }
                }
                if (lpString != NULL)
                {
                    uLength = strlen(lpString) + 1;
                    if (uSize > uLength + 1)
                    {
                        lpBuffer += wsprintf(lpBuffer, "%s ", lpString);
                    }
                    uSize -= uLength;
                    lpString = NULL;
                }
                ++uString;
                while (lpData[uIndex++]);
            } while (lpData[uIndex] && uIndex < uEnd);
            ++uIndex;
        }
    }

    if (lpData)
    {
        HeapFree(HandleHeap, 0, lpData);
    }
    SetLastError(lError);

    return uSize;

}

BOOL SystemInfo::Is64BitWindows()
{
#if defined(_WIN64)
    return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
    // 32-bit programs run on both 32-bit and 64-bit Windows
    // so must sniff
    BOOL f64 = FALSE;
    return IsWow64Process(GetCurrentProcess(), &f64) && f64;
#else
    return FALSE; // Win64 does not support Win16
#endif
}

std::string SystemInfo::GetNtDllVersion()
{
    std::string VersionString;
    DWORD dwFolder;
    char SystemFolderPath[_MAX_PATH + 1];
    UINT SystemFolderPathSize = GetSystemDirectory(SystemFolderPath, _MAX_PATH);
    if (SystemFolderPathSize > 0)
    {
        std::string ntDllPath(SystemFolderPath, SystemFolderPathSize);
        ntDllPath += "\\ntoskrnl.exe";
        DWORD dwVersionSize = GetFileVersionInfoSizeA(ntDllPath.c_str(), &dwFolder);
        if (dwVersionSize > 0)
        {
            LPVOID lpDataPtr = malloc(dwVersionSize);
            if (lpDataPtr != NULL)
            {
                if (GetFileVersionInfo(ntDllPath.c_str(), 0, dwVersionSize, lpDataPtr))
                {
                    UINT uLength;
                    LPVOID lpOutputPtr;
                    if (VerQueryValue(lpDataPtr, "\\", &lpOutputPtr, &uLength))
                    {
                        VS_FIXEDFILEINFO* VersionStructPtr = reinterpret_cast<VS_FIXEDFILEINFO*>(lpOutputPtr);
                        if (VersionStructPtr->dwSignature == 0xFEEF04BD)
                        {
                            VersionString = std::to_string(HIWORD(VersionStructPtr->dwFileVersionMS));
                            VersionString = std::to_string(LOWORD(VersionStructPtr->dwFileVersionMS));

                            VersionString = std::to_string(HIWORD(VersionStructPtr->dwFileVersionLS));
                            VersionString = std::to_string(LOWORD(VersionStructPtr->dwFileVersionLS));
                        }
                    }
                }
            }
        }
    }

    return VersionString;
}

std::string SystemInfo::GetLanguage()
{
    std::string LanguageString;
    char UserDefaultUILanguage = GetUserDefaultUILanguage();
    char SystemDefaultUILanguage = GetSystemDefaultUILanguage();
    int UserDefaultLangID = GetUserDefaultLangID();
    int SystemDefaultLangID = GetSystemDefaultLangID();
    int UserDefaultLCID = GetUserDefaultLCID();
    int SystemDefaultLCID = GetSystemDefaultLCID();

    char buffer[100];
    LCID lcid = UserDefaultLCID;

    if (GetLocaleInfo(lcid, LOCALE_ILANGUAGE, buffer, 100))
    {
        LanguageString = buffer;
    }
    if (GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, buffer, 100))
    {
        LanguageString = buffer;
    }
    if (GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, buffer, 100))
    {
        LanguageString = buffer;
    }

    return LanguageString;
}
