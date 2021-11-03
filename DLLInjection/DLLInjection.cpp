#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcID(const wchar_t* procName)
{
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}

	CloseHandle(hSnap);
	return procID;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error: require 1 args, passed %d args", argc - 1);
		exit(-1);
	}

	const char* dllPath = argv[1];
	
	DWORD procID = 0;

	while (!procID)
	{
		procID = GetProcID(L"hl2.exe");
		std::cout << procID << std::endl;
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

	if (hProc != INVALID_HANDLE_VALUE)
	{
		void* allocatedMemory = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		WriteProcessMemory(hProc, allocatedMemory, dllPath, strlen(dllPath) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocatedMemory, 0, 0);

		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	CloseHandle(hProc);
	return 0;
}
