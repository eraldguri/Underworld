#include "CpuInfo.h"

CpuInfo::CpuUsage::CpuUsage() : m_nCpuUsage(-1), m_dwLastRun(0), m_lRunCount(0) 
{
	ZeroMemory(&m_ftPrevSysKernel, sizeof(FILETIME));
	ZeroMemory(&m_ftPrevSysUser, sizeof(FILETIME));

	ZeroMemory(&m_ftPrevProcKernel, sizeof(FILETIME));
	ZeroMemory(&m_ftPrevProcUser, sizeof(FILETIME));
}

short CpuInfo::CpuUsage::GetUsage()
{
	// Create a local copy to protected against race conditions in setting the member variable
	short nCpuCopy = m_nCpuUsage;
	if (InterlockedIncrement(&m_lRunCount) == 1)
	{
		// If this is called too often, the measurement itself will greatly affect the results.
		if (!EnoughTimePassed())
		{
			InterlockedDecrement(&m_lRunCount);
			return nCpuCopy;
		}

		FILETIME ftSysIdle, ftSysKernel, ftSysUser;
		FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;
		if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser))
		{
			InterlockedDecrement(&m_lRunCount);
			return nCpuCopy;
		}

		if (!IsFirstRun())
		{
			// CPU usage us calculated by getting the total amount of time the process has run (kernel + user)
			// over the total amount of time the system has operated since the last measurement (kernel + user).
			ULONGLONG ftSysKernelDiff	= SubStractTimes(ftSysKernel, m_ftPrevSysKernel);
			ULONGLONG ftSysUserDiff = SubStractTimes(ftSysUser, m_ftPrevSysUser);
			ULONGLONG ftProcKernelDiff = SubStractTimes(ftProcKernel, m_ftPrevProcKernel);
			ULONGLONG ftProcUserDiff = SubStractTimes(ftProcUser, m_ftPrevProcUser);
			ULONGLONG nTotalSys = ftSysKernelDiff + ftSysUserDiff;
			ULONGLONG nTotalProc = ftProcKernelDiff + ftProcUserDiff;
			if (nTotalSys > 0)
			{
				m_nCpuUsage = (short)((100.0 * nTotalProc) / nTotalSys);
			}
		}

		m_ftPrevSysKernel = ftSysKernel;
		m_ftPrevSysUser = ftSysUser;
		m_ftPrevProcKernel = ftProcKernel;
		m_ftPrevProcUser = ftProcUser;

		m_dwLastRun = GetTickCount64();
		nCpuCopy = m_nCpuUsage;
	}

	InterlockedDecrement(&m_lRunCount);

	return nCpuCopy;
}

ULONGLONG CpuInfo::CpuUsage::SubStractTimes(const FILETIME& fileTimeA, const FILETIME& fileTimeB)
{
	LARGE_INTEGER a, b;

	a.LowPart = fileTimeA.dwLowDateTime;
	a.HighPart = fileTimeA.dwHighDateTime;

	b.LowPart = fileTimeB.dwLowDateTime;
	b.HighPart = fileTimeB.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}

bool CpuInfo::CpuUsage::EnoughTimePassed()
{
	const int minElapsedMS = MIN_CPU_USAGE_CHECK_INTERVAL;
	ULONGLONG dwCurrentTickCount = GetTickCount64();

	return (dwCurrentTickCount - m_dwLastRun) > minElapsedMS;
}
