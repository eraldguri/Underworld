#include "MemoryInfo.h"
#include "SysUtils.h"
#include <sstream>

MemoryInfo::MemoryInfo()
{
	RetrieveMemoryInfo();
}

MemoryInfo::~MemoryInfo()
{
}

void MemoryInfo::RetrieveMemoryInfo()
{
	GlobalMemoryStatus(&m_memoryStatus);

	m_stringTotalRam = SysUtils::ByteToString(m_memoryStatus.dwTotalPhys + 655360, 0);
	m_stringAvailableRam = SysUtils::ByteToString(m_memoryStatus.dwAvailPhys + 655360);
	m_stringTotalPageFile = SysUtils::ByteToString(m_memoryStatus.dwTotalPageFile + 655360);
	m_stringAvailablePageFile = SysUtils::ByteToString(m_memoryStatus.dwAvailPageFile + 655360);
	m_stringTotalVirtualMemory = SysUtils::ByteToString(m_memoryStatus.dwTotalVirtual + 655360);
	m_stringTotalAvailableVirtualMemory = SysUtils::ByteToString(m_memoryStatus.dwAvailVirtual + 655360);
}

std::string MemoryInfo::GetTotalRam()
{
	return m_stringTotalRam;
}

std::string MemoryInfo::GetAvailableRam() const
{
	return m_stringAvailableRam;
}

std::string MemoryInfo::GetTotalPageFile() const
{
	return m_stringTotalPageFile;
}

std::string MemoryInfo::GetAvailablePageFile() const
{
	return m_stringAvailablePageFile;
}

std::string MemoryInfo::GetTotalVirtualMemory() const
{
	return m_stringTotalVirtualMemory;
}

std::string MemoryInfo::GetAvailableVirtualMemory() const
{
	return m_stringTotalAvailableVirtualMemory;
}
