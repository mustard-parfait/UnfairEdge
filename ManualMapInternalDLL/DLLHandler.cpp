#include "Include.h"

// Set initial dll file info
DLLHandler::DLLHandler(const char* DllPath)
{
	// Set dll file and open file stream
	DLLFile = DllPath;
	File.open(DLLFile, std::ios::binary | std::ios::ate);
	if (File.fail())
	{
		printf("Error: failed to read file\n");
		exit(-1);
	}

	// Get file size and quit if < 4096
	FileSize = File.tellg();
	if (FileSize < 4096)
	{
		File.close();
		exit(-1);
	}

	// Declare memory space for dll data
	dllData = new BYTE[FileSize];
	if (!dllData)
	{
		File.close();
		exit(-1);
	}

	// Read dll data
	File.seekg(0, std::ios::beg);
	File.read((char*)dllData, FileSize);
	File.close();

	// Check if file is a windows dll or not
	if (!CheckDLL())
	{
		File.close();
		delete[] dllData;
		exit(-1);
	}

	// Get dll NT Headers
	dllNtHeaders = (IMAGE_NT_HEADERS*)(dllData + ((IMAGE_DOS_HEADER*)dllData)->e_lfanew);

	// Get dll Optional Header
	dllOptionalHeader = &dllNtHeaders->OptionalHeader;
	
	// Get dll File Header
	dllFileHeader = &dllNtHeaders->FileHeader;

	// Check if valid architecture or not
	if (!CheckArch())
	{
		File.close();
		FreeBuffer();
		exit(-1);
	}
}

// Check dll
bool DLLHandler::CheckDLL()
{
	if (((IMAGE_DOS_HEADER*)dllData)->e_magic == 0x5A4D)
		return true;
	else
		return false;
}

// Check dll architecture - x64 - AMD64, x86 - i386
bool DLLHandler::CheckArch()
{
	if (dllFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64)
		return true;
	else if (dllFileHeader->Machine == IMAGE_FILE_MACHINE_I386)
		return true;
	else
		return false;
}

// Free buffer
void DLLHandler::FreeBuffer()
{
	delete[] dllData;
}