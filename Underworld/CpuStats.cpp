#include "CpuStats.h"

const std::string CCpuStats::VENDOR_INTEL_STRING;
const std::string CCpuStats::VENDOR_AMD_STRING;
const std::string CCpuStats::VENDOR_CYRIX_STRING;
const std::string CCpuStats::VENDOR_CENTAUR_STRING;

CCpuStats::CCpuStats()
{
	SetKnowsCpuId(false);
}

CCpuStats::CCpuStats(const CCpuStats& source)
{
}

CCpuStats& CCpuStats::operator=(const CCpuStats& right)
{
	// // O: insert return statement here
	return NULL;
}

CCpuStats::~CCpuStats()
{
}

bool CCpuStats::GetKnowsCpuId() const
{
	return m_bCpuIdSupported;
}

void CCpuStats::SetKnowsCpuId(const bool& bKnowsCpuId)
{
	m_bCpuIdSupported = bKnowsCpuId;
}

DWORD CCpuStats::GetSignature() const
{
	return m_cpuIdStats.GetEaxValue();
}

void CCpuStats::SetSignature(const DWORD& dwSignature)
{
	m_cpuIdStats.SetEaxValue(dwSignature);
}

int CCpuStats::GetFamily() const
{
	return ((GetSignature() >> 8) & 0x0f);
}

int CCpuStats::GetModel() const
{
	return ((GetSignature() >> 4) & 0x0f);
}

int CCpuStats::GetStepping() const
{
	return (GetSignature() & 0x0f);
}

std::string CCpuStats::GetName() const
{
	return m_nameString;
}

void CCpuStats::SetName(std::string& name)
{
	m_nameString = name;
}

std::string CCpuStats::GetVendorId() const
{
	return m_vendorIdString;
}

CCpuStats::ECpuVendors CCpuStats::GetVendor() const
{
	return m_eVendor;
}
