#include "Include.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error: require 1 args, passed %d args", argc - 1);
		exit(-1);
	}

	// Process name and dll path
	const wchar_t* ProcName = L"hl2.exe";
	const char* DLLPath = argv[1];

	// Get process ID
	DWORD ProcID = 0;
	while (ProcID == 0)
	{
		ProcID = GetProcID(ProcName);
	}

	// Open process and get handle
	HANDLE HProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
	if (HProc == INVALID_HANDLE_VALUE)
	{
		printf("Error: failed to open process\n");
		exit(-1);
	}

	// Manual map passed dll into specified process
	if (!ManualMap(HProc, DLLPath))
	{
		printf("Error: failed to map dll into process\n");
		exit(-1);
	}
	printf("Manually Mapped InternalDLL.dll\n");
	getchar();
	return 0;
}
