#include "Include.h"

void __stdcall Concode(MANUAL_MAPPING_DATA* mapData)
{
	// If map data if NULL return
	if (!mapData)
		return;

	// Get base address and optional headers
	BYTE* MapBase = reinterpret_cast<BYTE*>(mapData);
	auto* MapOptHeader = &reinterpret_cast<IMAGE_NT_HEADERS*>(MapBase + reinterpret_cast<IMAGE_DOS_HEADER*>(mapData)->e_lfanew)->OptionalHeader;

	// Get pointers of LoadLibraryA and GetProcAddress (avoid calling functions)
	auto funcLoadLibraryA = mapData->dataLoadLibrary;
	auto funcGetProcAddress = mapData->dataGetProcAddress;
	auto DLLMain = reinterpret_cast<fnDLLMain>(MapBase + MapOptHeader->AddressOfEntryPoint);

	// Check if relocation is neeeded
	BYTE* DiffBase = MapBase - MapOptHeader->ImageBase;
	if (DiffBase)
	{
		// Check if relocatable
		if (!MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
			return;

		// Relocate addresses using RELOCATION_TABLE
		auto* MapRelocation = reinterpret_cast<IMAGE_BASE_RELOCATION*>(MapBase + MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		// If relocation table consits of required relocation
		while (MapRelocation->VirtualAddress)
		{
			// Get number of relocations to be made (subtract size of each block)
			UINT NumberOfRelocations = (MapRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD* MapRelative = reinterpret_cast<WORD*>(MapRelocation + 1);

			// Loop through number of relocations and depending on type, change by offset
			for (UINT i = 0; i != NumberOfRelocations; ++i, ++MapRelative)
			{
				if (RELOC_FLAG(*MapRelative))
				{
					UINT_PTR* MapRelocate = reinterpret_cast<UINT_PTR*>(MapBase + MapRelocation->VirtualAddress + ((*MapRelative) & 0xFFF));
					*MapRelocate += reinterpret_cast<UINT_PTR>(DiffBase);
				}
			}

			// Move to next Relocation Table block
			MapRelocation = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(MapRelocation) + MapRelocation->SizeOfBlock);
		}
	}

	// Resolve imports by looking up required libraries, functions and loading them (check if there are any required imports)
	if (MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		// Get initial pointer to import descriptor structure
		auto* MapImportDesp = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(MapBase + MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		// While there is a module to load
		while (MapImportDesp->Name)
		{
			// Get module name
			char* ModuleName = reinterpret_cast<char*>(MapBase + MapImportDesp->Name);

			// Load library using LoadLibraryA prototype
			HINSTANCE HDLL = funcLoadLibraryA(ModuleName);

			// Get first thunk and original thunk
			ULONG_PTR* MapOriginalThunk = reinterpret_cast<ULONG_PTR*>(MapBase + MapImportDesp->OriginalFirstThunk);
			ULONG_PTR* MapFirstThunk = reinterpret_cast<ULONG_PTR*>(MapBase + MapImportDesp->FirstThunk);

			// If original thunk is null, use first thunk as original
			if (!MapOriginalThunk)
				MapOriginalThunk = MapFirstThunk;

			// Loop through thunks to get libraries and functions - with help of oridinals (for example ReadProcessMemory - Oridinal: 32)
			for (; *MapOriginalThunk; ++MapOriginalThunk, ++MapFirstThunk)
			{
				if (IMAGE_SNAP_BY_ORDINAL(*MapOriginalThunk))
				{
					*MapFirstThunk = funcGetProcAddress(HDLL, reinterpret_cast<char*>(*MapOriginalThunk & 0xFFFF));
				}
				else
				{
					auto* pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(MapBase + (*MapOriginalThunk));
					*MapFirstThunk = funcGetProcAddress(HDLL, pImport->Name);
				}
			}
			++MapImportDesp;
		}
	}

	// Resolve TLS callbacks (Thread Local Storage) - check if any entries are available
	if (MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
	{
		// Get init pointer to TLS directory
		auto* MapTLS = reinterpret_cast<IMAGE_TLS_DIRECTORY*>(MapBase + MapOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

		// Get address of callbacks
		auto* MapCallbacks = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(MapTLS->AddressOfCallBacks);

		// Loop through all callbacks and initiate
		for (; MapCallbacks && *MapCallbacks; ++MapCallbacks)
			(*MapCallbacks)(MapBase, DLL_PROCESS_ATTACH, nullptr);
	}

	// Call DLL MAIN to start the dll process
	DLLMain(MapBase, DLL_PROCESS_ATTACH, nullptr);

	// Assign module for checking
	mapData->hModule = reinterpret_cast<HINSTANCE>(MapBase);
}