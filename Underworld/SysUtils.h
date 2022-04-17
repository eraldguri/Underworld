#pragma once

#include "Win32Settings.h"

class SysUtils
{
private:
	SysUtils();
	virtual ~SysUtils();
	SysUtils(const SysUtils& source);
	SysUtils& operator=(const SysUtils& right);

public:
	static std::string ByteToString(const __int64& n64ByteCount = 0, const int& nPrecision = 2);
};