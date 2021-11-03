#include "pch.h"
#include "Include.h"

LocalPlayer* LocalPlayerEntity = NULL;

// Fake create move function
bool __fastcall fakeCreateMove(float sampletime, CUserCmd* usercmd)
{
	if (LocalPlayerEntity != NULL)
	{
		Vec3 angles = *LocalPlayerEntity->GetAngles();
		usercmd->viewangles.x = angles.x;
		usercmd->viewangles.y = angles.y;
	}

	return true;
}

// Set local player for CreateMove angles
Hooking::Hooking(LocalPlayer* lpPlayer)
{
	LocalPlayerEntity = lpPlayer;
}

// Set initial constructor
Hooking::Hooking()
{
	LocalPlayerEntity = nullptr;
}

// Set local player after initialization
void Hooking::SetLocalPlayer(LocalPlayer* lpPlayer)
{
	LocalPlayerEntity = lpPlayer;
}

// Writing bytes to dst from src
void Hooking::Patch(BYTE* dst, BYTE* src, DWORD size)
{
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

// Inline hooking (jmp address)
bool Hooking::InlineHook(char* src, char* dst, DWORD size)
{
	if (size <= 5) return false;

	DWORD oldProtect;
	VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(src, 0x90, size);
	uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeAddress;
	VirtualProtect(src, size, oldProtect, &oldProtect);
	return true;
}

// Trampoling hooking - advantage of gateway
char* Hooking::TrampolineHook(char* src, char* dst, DWORD size)
{
	if (size <= 5) return 0;

	char* callbackIns = (char*)VirtualAlloc(0, size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (callbackIns == 0) return 0;
	
	memcpy(callbackIns, src, size);
	uintptr_t jumpHookAddress = (uintptr_t)(src - callbackIns - 5);
	*(callbackIns + size) = (char)0xE9;
	*(uintptr_t*)(callbackIns + size + 1) = jumpHookAddress;
	if (InlineHook(src, dst, size))
		return callbackIns;
	else
		return 0;
}