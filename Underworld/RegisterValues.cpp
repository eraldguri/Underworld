#include "RegisterValues.h"

RegisterValues::RegisterValues()
{
	SetEaxValue(0);
	SetEbxValue(0);
	SetEcxValue(0);
	SetEdxValue(0);
}

RegisterValues::RegisterValues(const DWORD& dwEax, const DWORD& dwEbx, const DWORD& dwEcx, const DWORD& dwEdx)
{
	SetEaxValue(dwEax);
	SetEbxValue(dwEbx);
	SetEcxValue(dwEcx);
	SetEdxValue(dwEdx);
}

RegisterValues::RegisterValues(const RegisterValues& source)
{
	Assign(source);
}

RegisterValues& RegisterValues::operator=(const RegisterValues& right)
{
	if (this != &right)
	{
		Assign(right);
	}

	return (*this);
}

RegisterValues::~RegisterValues()
{
}

DWORD RegisterValues::GetEaxValue() const
{
	return m_dwEaxValue;
}

void RegisterValues::SetEaxValue(const DWORD& dwEax)
{
	m_dwEaxValue = dwEax;
}

DWORD RegisterValues::GetEbxValue() const
{
	return m_dwEbxValue;
}

void RegisterValues::SetEbxValue(const DWORD& dwEbx)
{
	m_dwEbxValue = dwEbx;
}

DWORD RegisterValues::GetEcxValue() const
{
	return m_dwEcxValue;
}

void RegisterValues::SetEcxValue(const DWORD& dwEcx)
{
	m_dwEcxValue = dwEcx;
}

DWORD RegisterValues::GetEdxValue() const
{
	return m_dwEdxValue;
}

void RegisterValues::SetEdxValue(const DWORD& dwEdx)
{
	m_dwEdxValue = dwEdx;
}

void RegisterValues::Assign(const RegisterValues& source)
{
	SetEaxValue(source.GetEaxValue());
	SetEbxValue(source.GetEbxValue());
	SetEcxValue(source.GetEcxValue());
	SetEdxValue(source.GetEdxValue());
}
