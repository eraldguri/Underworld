#pragma once

#include "Win32Settings.h"

// this gives us multimedia timer functions -- used for CPU benchmarking
#pragma comment(lib, "winmm.lib")

namespace CpuInfo 
{
	
class CCpuInfo
{
public:
	CCpuInfo(const bool fCpuSpeed = true);
	CCpuInfo(const CCpuInfo& source);
	CCpuInfo& operator=(const CCpuInfo& right);
	virtual ~CCpuInfo();
};

}