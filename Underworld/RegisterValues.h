#pragma once

#include "Win32Settings.h"

class RegisterValues
{
public:
	RegisterValues();
	RegisterValues(const DWORD& dwEax, const DWORD& dwEbx, const DWORD& dwEcx, const DWORD& dwEdx);
	RegisterValues(const RegisterValues& source);
	RegisterValues& operator=(const RegisterValues& right);
	virtual ~RegisterValues();

public:
	virtual DWORD GetEaxValue() const;
	virtual void SetEaxValue(const DWORD& dwEax);

	virtual DWORD GetEbxValue() const;
	virtual void SetEbxValue(const DWORD& dwEbx);

	virtual DWORD GetEcxValue() const;
	virtual void SetEcxValue(const DWORD& dwEcx);

	virtual DWORD GetEdxValue() const;
	virtual void SetEdxValue(const DWORD& dwEdx);

protected:
	virtual void Assign(const RegisterValues& source);

private:
	DWORD m_dwEaxValue;
	DWORD m_dwEbxValue;
	DWORD m_dwEcxValue;
	DWORD m_dwEdxValue;
};