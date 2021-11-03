#pragma once

#define RELOC_FLAG32(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_HIGHLOW)
#define RELOC_FLAG64(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)

#ifdef _WIN64
#define RELOC_FLAG RELOC_FLAG64
#else
#define RELOC_FLAG RELOC_FLAG32
#endif

// Declare function prototypes

/*
	LoadLibraryA
	HMODULE LoadLibraryA(
		LPCSTR lpLibFileName
	);
*/
using fnLoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibFileName);

/*
	GetProcAddress
	FARPROC GetProcAddress(
		HMODULE hModule,
		LPCSTR  lpProcName
	);
*/
using fnGetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char* lpProcName);

/*
	BOOL WINAPI DllMain(
		HINSTANCE hinstDLL,
		DWORD fdwReason,
		LPVOID lpReserved
	)
*/
using fnDLLMain = BOOL(WINAPI*)(void* hDLL, DWORD dwReason, void* pReserverd);

// Manual mapping required data
typedef struct {
	fnLoadLibraryA dataLoadLibrary;
	fnGetProcAddress dataGetProcAddress;
	HINSTANCE hModule;
} MANUAL_MAPPING_DATA;

// Manual map dll into passed Process Handle
bool ManualMap(HANDLE hProc, const char* szDllFile);