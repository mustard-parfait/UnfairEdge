#pragma once

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

class SDK
{
public:
	// Process Information
	const wchar_t* ProcName;
	DWORD ProcID = 0;
	HANDLE hProc = 0;

	// Required modules address and sizes
	uintptr_t ClientBase = 0;
	uintptr_t EngineBase = 0;
	DWORD ClientSize = 0;
	DWORD EngineSize = 0;

	// Process handlers
	SDK(const wchar_t* ProcName);
	DWORD GetProcID();
	HANDLE GetProcHandle();
	uintptr_t GetModuleBaseAddress(const wchar_t* ModuleName);
	DWORD GetModuleSize(const wchar_t* ModuleName);
	uintptr_t MultiPointerAddress(uintptr_t InitAddress, std::vector<unsigned int> pointers);

	// Signature Scanning
	bool CompareBytes(BYTE* data, BYTE* sig, const char* mask);
	uintptr_t SignatureScan(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask);

	// Offsets with signatures
	uintptr_t GetOffsetAddress(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask, DWORD moveBytes);
	uintptr_t GetOffsets(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask, DWORD moveBytes);
	uintptr_t GetClientOffsets(BYTE* sig, const char* mask, DWORD moveBytes);
	uintptr_t GetEngineOffsets(BYTE* sig, const char* mask, DWORD moveBytes);
	uintptr_t GetStaticOffsets(BYTE* sig, const char* mask, DWORD moveByte);
};