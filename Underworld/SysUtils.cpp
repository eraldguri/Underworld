#include "SysUtils.h"
#include <sstream>

std::string SysUtils::ByteToString(const __int64& n64ByteCount, const int& nPrecision)
{
	constexpr int NUMFORMATTERS = 5;
	char szSizeFormatters[NUMFORMATTERS][10] = { " bytes", " KB", " MB", " GB", " TB" };
	double dblBase = n64ByteCount;
	int nNumConversions = 0;

	while (dblBase > 1000)
	{
		dblBase /= 1024;
		nNumConversions++;
	}

	std::string UnitsString;
	if ((nNumConversions >= 0) && (nNumConversions <= NUMFORMATTERS))
	{
		UnitsString = szSizeFormatters[nNumConversions];
	}

	std::ostringstream StringStream;
	StringStream.setf(std::ostringstream::fixed);
	StringStream.precision(nPrecision);
	StringStream << dblBase << UnitsString << std::ends;

	return StringStream.str();
}
