#include "Include.h"

// Get process ID of the process
DWORD GetProcID(const wchar_t* ProcName)
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
				if (!_wcsicmp((LPWSTR)ProcName, ProcEntry.szExeFile))
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
