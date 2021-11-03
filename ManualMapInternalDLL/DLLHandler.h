#pragma once

// Required information for mapping dll into target process
class DLLHandler
{
public:
	// Basic DLL info
	const char* DLLFile;
	std::ifstream File;
	DWORD FileSize;

	// Dll info (data, PE headers)
	BYTE* dllData = nullptr;
	IMAGE_NT_HEADERS* dllNtHeaders = nullptr;
	IMAGE_OPTIONAL_HEADER* dllOptionalHeader = nullptr;
	IMAGE_FILE_HEADER* dllFileHeader = nullptr;
	IMAGE_SECTION_HEADER* dllSectionHeader = nullptr;

	DLLHandler(const char* DllPath);
	bool CheckDLL();
	bool CheckArch();
	void FreeBuffer();
};