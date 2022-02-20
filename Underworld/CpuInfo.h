#pragma once

#include "Win32Settings.h"

namespace CpuInfo 
{
	constexpr int MIN_CPU_USAGE_CHECK_INTERVAL = 250;
	constexpr int APP_TICKS_INTERVAL = 1000;

	class CpuUsage
	{
	public:
		CpuUsage();

		/*
		*  Returns the percent of the CPU usage which this process has used since
		*  the last time this method was called.
		* 
		*	It returns -1 if there is not enough information, or called for the first time.
		*	It returns the previous returned value if it is called "too soon" from the last call;
		*/
		short GetUsage();

	private:
		/*
		* Evaluates the time difference between two file times.
		*/
		ULONGLONG SubStractTimes(const FILETIME& fileTimeA, const FILETIME& fileTimeB);

		/*
		* Checks if we check the timer too often.
		*/
		bool EnoughTimePassed();

		inline bool IsFirstRun() const { return m_dwLastRun == 0; }
	
		FILETIME m_ftPrevSysKernel;
		FILETIME m_ftPrevSysUser;
		FILETIME m_ftPrevProcKernel;
		FILETIME m_ftPrevProcUser;

		short m_nCpuUsage;
		ULONGLONG m_dwLastRun;

		volatile LONG m_lRunCount;
	};

}