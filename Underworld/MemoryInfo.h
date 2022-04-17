#pragma once
#include "Win32Settings.h"
#include <string>

class MemoryInfo
{
public:
	MemoryInfo();
	MemoryInfo(const MemoryInfo& source);
	MemoryInfo& operator=(const MemoryInfo& right);
	virtual ~MemoryInfo();

	void RetrieveMemoryInfo();

	std::string GetTotalRam();

	std::string GetAvailableRam() const;

	std::string GetTotalPageFile() const;

	std::string GetAvailablePageFile() const;

	std::string GetTotalVirtualMemory() const;

	std::string GetAvailableVirtualMemory() const;

private:
	MEMORYSTATUS m_memoryStatus;
	std::string m_stringTotalRam;
	std::string m_stringAvailableRam;
	std::string m_stringTotalPageFile;
	std::string m_stringAvailablePageFile;
	std::string m_stringTotalVirtualMemory;
	std::string m_stringTotalAvailableVirtualMemory;
};