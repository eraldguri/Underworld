#pragma once
#include "Win32Settings.h"
#include "CpuInfo.h"
#include "RegisterValues.h"

namespace CpuFeatures
{
	#define FPU_FLAG              0x00000001     // [Bit 00] FPU onboard?                                
	#define VME_FLAG              0x00000002     // [Bit 01] Virtual Mode Extension                      
	#define DE_FLAG               0x00000004     // [Bit 02] Debugging Extension                         
	#define PSE_FLAG              0x00000008     // [Bit 03] Page Size Extension                         
	#define TSC_FLAG              0x00000010     // [Bit 04] Time Stamp Counter                          
	#define MSR_FLAG              0x00000020     // [Bit 05] Model Specific Registers                    
	#define PAE_FLAG              0x00000040     // [Bit 06] Physical Address Extension                  
	#define MCE_FLAG              0x00000080     // [Bit 07] Machine Check Exception                     
	#define CX8_FLAG              0x00000100     // [Bit 08] CMPXCHG8 instruction supported              
	#define APIC_FLAG             0x00000200     // [Bit 09] On-chip APIC hardware supported             
	#define RESERVED_10           0x00000400     // [Bit 10] reserved                             
	#define SEP_FLAG              0x00000800     // [Bit 11] Fast System Call                            
	#define MTRR_FLAG             0x00001000     // [Bit 12] Memory Type Range Registers                 
	#define PGE_FLAG              0x00002000     // [Bit 13] Page Global Enable                          
	#define MCA_FLAG              0x00004000     // [Bit 14] Machine Check Architecture                  
	#define CMOV_FLAG             0x00008000     // [Bit 15] Conditional Move Instruction Supported      
	#define PAT_FLAG              0x00010000     // [Bit 16] Page Attribute Table                        
	#define PSE36_FLAG            0x00020000     // [Bit 17] 36-bit Page Size Extension                  
	#define PSNUM_FLAG            0x00040000     // [Bit 18] Processor Serial Number Present and Enabled 
	#define CLFLUSH_FLAG          0x00080000     // [Bit 19] CLFLUSH instruction supported               
	#define RESERVED_20           0x00100000     // [Bit 20] reserved                             
	#define DTS_FLAG              0x00200000     // [Bit 21] Debug Store                                 
	#define ACPI_FLAG             0x00400000     // [Bit 22] Thermal Monitor and Software Clock          
	#define MMX_FLAG              0x00800000     // [Bit 23] MMX supported                               
	#define FXSR_FLAG             0x01000000     // [Bit 24] Fast Floating Point Save and Restore        
	#define SSE_FLAG              0x02000000     // [Bit 25] Streaming SIMD Extensions Supported          
	#define SSE2_FLAG             0x04000000     // [Bit 26] Streaming SIMD Extensions 2                 
	#define SS_FLAG               0x08000000     // [Bit 27] Self-Snoop                                  
	#define RESERVED_28           0x10000000     // [Bit 28] reserved                             
	#define TM_FLAG               0x20000000     // [Bit 29] Thermal Monitor Supported                   
	#define RESERVED_30           0x40000000     // [Bit 30] reserved                             
	#define RESERVED_31           0x80000000     // [Bit 31] reserved                             

			// extended feature #define's
	#define SSEMMX_FLAG           0x00400000     // [Bit 22] SSE MMX Extensions
	#define HAS3DNOW_FLAG         0x40000000     // [Bit 30] 3dNow!
	#define EXT3DNOW_FLAG         0x80000000     // [Bit 31] 3dNow! Extensions

	// largest extended feature #define's
	#define AMD_EXTENDED_FEATURE  0x80000001     // this gets extended processor features for AMD CPUs
	#define NAMESTRING_FEATURE    0x80000004     // this is the namestring feature; goes from 0x80000002 to 0x80000004
	#define AMD_L1CACHE_FEATURE   0x80000005     // this gets L1 cache info for AMD CPUs
	#define AMD_L2CACHE_FEATURE   0x80000006     // this gets L2 cache info for AMD CPUs
}

class CCpuStats
{
public:
	CCpuStats();
	CCpuStats(const CCpuStats& source);
	CCpuStats& operator=(const CCpuStats& right);
	virtual ~CCpuStats();

public:
	enum ECpuVendors
	{
		VENDOR_UNKNOWN = 0,
		VENDOR_INTEL,
		VENDOR_AMD,
		VENDOR_CYRIX,
		VENDOR_CENTAUR
	};

	friend class CCpuInfo;

public:
	// Cpu identification
	virtual bool GetKnowsCpuId() const;
	virtual void SetKnowsCpuId(const bool& bKnowsCpuId);
	virtual DWORD GetSignature() const;
	virtual void SetSignature(const DWORD& dwSignature);

	int GetFamily() const;
	int GetModel() const;
	int GetStepping() const;

	std::string GetName() const;
	void SetName(std::string& name);

	// vendor identification
	std::string GetVendorId() const;
	ECpuVendors GetVendor() const;

protected:
	void Assign(const CCpuStats& source);

private:
	bool m_bCpuIdSupported;				// Whether opcode CPU_ID is supported -- late 486 and after
	DWORD m_dwExtendedFeatures;			// AMD CPUs have extended features to test for things like 3dNow!
	std::string m_vendorIdString;       // Vendor Id in string form
	std::string m_nameString;           // name string -- either returned by modern CPU's or determined for them
	ECpuVendors m_eVendor;				// vendor id in enum form

	// Registers from CPUID-1
	RegisterValues m_cpuIdStats;
	RegisterValues m_cacheStats;
	RegisterValues m_cacheStats2;

	static const std::string VENDOR_INTEL_STRING;
	static const std::string VENDOR_AMD_STRING;
	static const std::string VENDOR_CYRIX_STRING;
	static const std::string VENDOR_CENTAUR_STRING;
};