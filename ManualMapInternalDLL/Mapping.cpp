#include "Include.h"

bool ManualMap(HANDLE hProc, const char* DLLPath)
{
	// Initiate dll handler
	DLLHandler* dll = new DLLHandler(DLLPath);

	// Get target base for writing dll data (ImageBase - preferred base of the file)
	BYTE* ProcessBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, reinterpret_cast<void*>(dll->dllOptionalHeader->ImageBase), dll->dllOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	// If ImageBase not avaliable
	if (!ProcessBase)
	{
		ProcessBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, nullptr, dll->dllOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!ProcessBase)
		{
			printf("Error: allocating memory for dll failed\n");
			dll->FreeBuffer();
			return false;
		}
	}

	// Initiate required data for manual mapping
	MANUAL_MAPPING_DATA MapData{ 0 };
	MapData.dataLoadLibrary = LoadLibraryA;
	MapData.dataGetProcAddress = reinterpret_cast<fnGetProcAddress>(GetProcAddress);

	// Map sections from dll into ProcessBase (read first section using IMAGE_FIRST_SECTION)
	dll->dllSectionHeader = IMAGE_FIRST_SECTION(dll->dllNtHeaders);
	for (UINT i = 0; i != dll->dllFileHeader->NumberOfSections; ++i, ++dll->dllSectionHeader)
	{
		// If section contains
		if (dll->dllSectionHeader->SizeOfRawData)
		{
			if (!WriteProcessMemory(hProc, ProcessBase + dll->dllSectionHeader->VirtualAddress, dll->dllData + dll->dllSectionHeader->PointerToRawData, dll->dllSectionHeader->SizeOfRawData, nullptr))
			{
				printf("Error: failed to map sections\n");
				dll->FreeBuffer();
				VirtualFreeEx(hProc, ProcessBase, 0, MEM_RELEASE);
				return false;
			}
		}
	}

	// Copy manual mapping data into dllData
	memcpy(dll->dllData, &MapData, sizeof(MapData));

	// Write dllData to ProcessBase
	if (!WriteProcessMemory(hProc, ProcessBase, dll->dllData, 0x1000, nullptr))
	{
		printf("Error: failed to write dll data into memory\n");
		VirtualFreeEx(hProc, ProcessBase, 0, MEM_RELEASE);
		return false;
	}

	// Free dll source data
	dll->FreeBuffer();

	// Allocate and write shell code to memory
	void* dllConcode = VirtualAllocEx(hProc, 0, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!dllConcode)
	{
		printf("Error: failed allocating memory for shell code\n");
		VirtualFreeEx(hProc, ProcessBase, 0, MEM_RELEASE);
		return false;
	}

	WriteProcessMemory(hProc, dllConcode, Concode, 0x1000, nullptr);

	// Initial a remote thread to start the dll main
	HANDLE hThread = CreateRemoteThread(hProc, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(dllConcode), ProcessBase, 0, 0);
	if (!hThread)
	{
		printf("Error: failed to create a remote thread for execution\n");
		VirtualFreeEx(hProc, ProcessBase, 0, MEM_RELEASE);
		VirtualFreeEx(hProc, dllConcode, 0, MEM_RELEASE);
		return false;
	}

	// Avoiding to close as soon as it is initiated
	WaitForSingleObject(hThread, 30000);
	CloseHandle(hThread);

	// Check if the module is loaded or not
	HINSTANCE dllCheck = NULL;
	while (!dllCheck)
	{
		MANUAL_MAPPING_DATA dllCheckData{ 0 };
		ReadProcessMemory(hProc, ProcessBase, &dllCheckData, sizeof(dllCheckData), 0);
		dllCheck = dllCheckData.hModule;
		Sleep(10);
	}

	// Free up memory by releasing shell code
	VirtualFreeEx(hProc, dllConcode, 0, MEM_RELEASE);
	return true;
}