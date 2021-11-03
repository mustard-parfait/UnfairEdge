#include "pch.h"
#include "Include.h"

void* GetInterface(const wchar_t* moduleName, const char* interfaceName)
{
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(moduleName), "CreateInterface");

	int returnCode = 0;
	void* moduleInterface = CreateInterface(interfaceName, &returnCode);
	return moduleInterface;
}

SDK::SDK()
{
	// Get Base Client
	BaseClientDLL = (IBaseClientDLL*)GetInterface(L"client.dll", "VClient017");
	clientClassAddr = BaseClientDLL->GetAllClasses();

	// Get Client Entity List
	ClientEntityList = (IClientEntityList*)GetInterface(L"client.dll", "VClientEntityList003");

	// Get Client Mode from VClient017 Interface
	ClientModeInterface = (void*)GetInterface(L"client.dll", "VClient017");
	ClientMode = **(void***)((*(uintptr_t**)ClientModeInterface)[10] + 0x5);

	// Get address of CreateMove from ClientMode
	CreateMoveAddress = (*(void***)ClientMode)[21];
	
	// Direct pointer to CreateMove
	// uintptr_t createMoveAddr = (uintptr_t)(GetModuleHandle(L"client.dll") + 0x1EAEF0);

	// Get original bytes of CreateMove
	memcpy(ogCreateMove, (char*)CreateMoveAddress, 7);
};

// Get client class address
ClientClass* SDK::GetClientClass()
{
	return clientClassAddr;
}

// Get player entity address
uintptr_t SDK::GetPlayerEntity(int num)
{
	return (uintptr_t)ClientEntityList->GetClientEntity(num);
}

// Get local player entity
uintptr_t SDK::GetLocalPlayerEntity()
{
	uintptr_t clientBase = (uintptr_t)GetModuleHandle(L"client.dll");
	uintptr_t lpPlayerPointer = clientBase + 0xC3D580;
	return *(uintptr_t*)lpPlayerPointer;
}

// Get coords from game to screen
bool SDK::GameToScreen(Vec3 Game, Vec2& Screen)
{
	// Get local player
	PlayerOffsets* pOffsets = new PlayerOffsets();
	LocalPlayer* lpPlayer = new LocalPlayer(GetPlayerEntity(1), pOffsets);
	if (lpPlayer == NULL) return false;

	// Get local player view matrix
	float* ViewMatrix = lpPlayer->GetViewMatrix();

	Vec4 Coords;
	Coords.x = Game.x * ViewMatrix[0] + Game.y * ViewMatrix[1] + Game.z * ViewMatrix[2] + ViewMatrix[3];
	Coords.y = Game.x * ViewMatrix[4] + Game.y * ViewMatrix[5] + Game.z * ViewMatrix[6] + ViewMatrix[7];
	Coords.z = Game.x * ViewMatrix[8] + Game.y * ViewMatrix[9] + Game.z * ViewMatrix[10] + ViewMatrix[11];
	Coords.w = Game.x * ViewMatrix[12] + Game.y * ViewMatrix[13] + Game.z * ViewMatrix[14] + ViewMatrix[15];

	if (Coords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = Coords.x / Coords.w;
	NDC.y = Coords.y / Coords.w;
	NDC.z = Coords.z / Coords.w;

	Screen.x = (WindowWidth / 2 * NDC.x) + (NDC.x + WindowWidth / 2);
	Screen.y = -(WindowHeight / 2 * NDC.y) + (NDC.y + WindowHeight / 2);
	return true;
}