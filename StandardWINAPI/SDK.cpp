#include "Include.h"

SDK::SDK(const wchar_t* ProcName)
{
	// Set process name
	this->ProcName = ProcName;

	// Get process ID by enumerating through current processes
	ProcID = GetProcID();
	if (ProcID == 0) exit(-1);

	// Get required module relative base address and sizes
	ClientBase = GetModuleBaseAddress(L"client.dll");
	ClientSize = GetModuleSize(L"client.dll");
	EngineBase = GetModuleBaseAddress(L"engine.dll");
	EngineSize = GetModuleSize(L"engine.dll");

	// Open process and get handle
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
	if (hProc == INVALID_HANDLE_VALUE) exit(-1);
}

// Get process ID of the process
DWORD SDK::GetProcID()
{
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 ProcEntry;
		ProcEntry.dwSize = sizeof(ProcEntry);

		if (Process32First(hSnap, &ProcEntry))
		{
			do
			{
				// Compares each process name with required process
				if (!_wcsicmp(ProcName, ProcEntry.szExeFile))
				{
					procID = ProcEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &ProcEntry));
		}
	}

	CloseHandle(hSnap);
	return procID;
}

// Get relative module address
uintptr_t SDK::GetModuleBaseAddress(const wchar_t* ModuleName)
{
	uintptr_t ModuleBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcID);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry;
		ModuleEntry.dwSize = sizeof(ModuleEntry);

		if (Module32First(hSnap, &ModuleEntry))
		{
			do
			{
				// Compares each module name in process with required module
				if (!_wcsicmp(ModuleEntry.szModule, ModuleName))
				{
					ModuleBaseAddress = (uintptr_t)ModuleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &ModuleEntry));
		}
	}

	CloseHandle(hSnap);
	return ModuleBaseAddress;
}

// Get module size
DWORD SDK::GetModuleSize(const wchar_t* ModuleName)
{
	DWORD ModuleSize = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcID);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry;
		ModuleEntry.dwSize = sizeof(ModuleEntry);

		if (Module32First(hSnap, &ModuleEntry))
		{
			do
			{
				// Compares each module name in process with required module
				if (!_wcsicmp(ModuleEntry.szModule, ModuleName))
				{
					ModuleSize = ModuleEntry.modBaseSize;
					break;
				}
			} while (Module32Next(hSnap, &ModuleEntry));
		}
	}

	CloseHandle(hSnap);
	return ModuleSize;
}

// Get process handle
HANDLE SDK::GetProcHandle()
{
	return hProc;
}

// Loop through pointer address by offsets
uintptr_t SDK::MultiPointerAddress(uintptr_t InitAddress, std::vector<unsigned int> pointers)
{
	uintptr_t OffsetAddress = InitAddress;

	for (DWORD i = 0; i < pointers.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)OffsetAddress, &OffsetAddress, sizeof(OffsetAddress), 0);
		OffsetAddress += pointers[i];
	}

	return OffsetAddress;
}

// Checks for bytes that match signature and mask
bool SDK::CompareBytes(BYTE* data, BYTE* sig, const char* mask)
{
	for (; *mask; mask++, data++, sig++)
	{
		if (*mask == 'x' && *data != *sig)
		{
			return false;
		}
	}

	return true;
}

// Get address of bytes that matched signature and mask
uintptr_t SDK::SignatureScan(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask)
{
	MEMORY_BASIC_INFORMATION m_memBasic = { 0 };
	DWORD offset = 0;

	while (offset < size)
	{
		VirtualQueryEx(hProc, (BYTE*)(m_dwBase + offset), &m_memBasic, sizeof(m_memBasic));

		if (m_memBasic.State != MEM_FREE)
		{
			BYTE* buffer = new BYTE[m_memBasic.RegionSize];
			ReadProcessMemory(hProc, (BYTE*)m_memBasic.BaseAddress, buffer, m_memBasic.RegionSize, 0);

			for (DWORD i = 0; i < m_memBasic.RegionSize; i++)
			{
				if (CompareBytes(buffer + i, sig, mask))
				{
					delete[] buffer;
					return (uintptr_t)m_memBasic.BaseAddress + i;
				}
			}

			if (buffer != NULL)
				delete[] buffer;
		}

		offset += m_memBasic.RegionSize;
	}

	return 0;
}

// Get offset address from module base passed
uintptr_t SDK::GetOffsetAddress(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask, DWORD moveBytes)
{
	uintptr_t currentAddress = SignatureScan(m_dwBase, size, sig, mask);
	uintptr_t offsetAddress = 0;
	// Read required address after moving specified number of bytes
	ReadProcessMemory(hProc, (BYTE*)(currentAddress + moveBytes), &offsetAddress, sizeof(offsetAddress), 0);
	return offsetAddress - m_dwBase;
}

uintptr_t SDK::GetOffsets(uintptr_t m_dwBase, DWORD size, BYTE* sig, const char* mask, DWORD moveBytes)
{
	uintptr_t currentAddress = SignatureScan(m_dwBase, size, sig, mask);
	uintptr_t offsetAddress = 0;
	// Read required address after moving specified number of bytes
	ReadProcessMemory(hProc, (BYTE*)(currentAddress + moveBytes), &offsetAddress, sizeof(offsetAddress), 0);
	return offsetAddress;
}

// Get address from client.dll
uintptr_t SDK::GetClientOffsets(BYTE* sig, const char* mask, DWORD moveBytes)
{
	return GetOffsets(ClientBase, ClientSize, sig, mask, moveBytes);
}

uintptr_t SDK::GetStaticOffsets(BYTE* sig, const char* mask, DWORD moveBytes)
{
	return GetOffsetAddress(ClientBase, ClientSize, sig, mask, moveBytes);
}

// Get addresses from engine.dll
uintptr_t SDK::GetEngineOffsets(BYTE* sig, const char* mask, DWORD moveBytes)
{
	return GetOffsetAddress(EngineBase, EngineSize, sig, mask, moveBytes);
}