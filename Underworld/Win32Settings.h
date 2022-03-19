#pragma once

#define _WIN32_WINNT 0X0601	
#include <sdkddkver.h>

/*
* The following #defines disable a bunch of unused windows stuff. If you
* get weird errors when trying to do some windows stuff, try removing some
* (or all) of these defines (it will increase build time thoug).
*/
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#ifdef _UNICODE
#define _T(x)      L ## x
#else /* _UNICODE */
#define _T(x)      x
#endif /* _UNICODE */

#include <CommCtrl.h>
#include <string>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")