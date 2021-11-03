#include "Include.h"

// Initiate all offsets from signatures
Offsets::Offsets(SDK* sdk)
{
	this->sdk = sdk;

	// Local Player offset
	LocalPlayer = sdk->GetStaticOffsets((PBYTE)"\xA1\x00\x00\x00\x00\x33\xC9\x83\xC4\x04", "x????xxxxx", 1);

	// Entity List offset
	PlayerEntityList = sdk->GetStaticOffsets((PBYTE)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x51\x83\xC8\x01", "x????xxxxxxx", 1) + 0x18;

	// Player Controllables offset
	PlayerControllables = sdk->GetStaticOffsets((PBYTE)"\x68\x00\x00\x00\x00\x8B\x40\x28\xFF\xD0\xA1", "x????xxxxxx", 1);

	// Force Jump offset
	m_dwForceJump = PlayerControllables + 0x20;

	// Force Duck offset
	m_dwForceDuck = PlayerControllables + 0x5C;

	// Force Attack1 offset
	m_dwForceAttack1 = PlayerControllables + 0x2C;

	// Force Attack2 offset
	m_dwForceAttack2 = PlayerControllables + 0x38;

	// Force Forward offset
	m_dwForceForward = PlayerControllables + 0x34;

	// Force Back offset
	m_dwForceBack = PlayerControllables + 0x40;

	// Vector Angles offset
	dwVecAngles = sdk->GetEngineOffsets((PBYTE)"\xD9\x1D\x00\x00\x00\x00\xD9\x46\x04", "xx????xxx", 2);
}

// Get local player entity
uintptr_t Offsets::GetLocalPlayer()
{
	uintptr_t LocalPlayerPointer = sdk->ClientBase + LocalPlayer;
	uintptr_t LocalPlayerEntity = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)LocalPlayerPointer, &LocalPlayerEntity, sizeof(LocalPlayerEntity), 0);
	return LocalPlayerEntity;
}

// Get player entity list
uintptr_t Offsets::GetPlayerEntityList()
{
	return sdk->ClientBase + PlayerEntityList;
}

// Get player entity from entity list
uintptr_t Offsets::GetPlayerEntity(int num)
{
	uintptr_t PlayerET = (sdk->ClientBase + PlayerEntityList) + num * 0x10;
	uintptr_t PlayerEntityAddress = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)PlayerET, &PlayerEntityAddress, sizeof(PlayerEntityAddress), 0);
	return PlayerEntityAddress;
}

// Get address for force jump
uintptr_t Offsets::GetForceJump()
{
	return sdk->ClientBase + m_dwForceJump;
}

// Get address for force duck
uintptr_t Offsets::GetForceDuck()
{
	return sdk->ClientBase + m_dwForceDuck;
}

// Get address for force attack1
uintptr_t Offsets::GetForceAttack1()
{
	return sdk->ClientBase + m_dwForceAttack1;
}

// Get address for force attack2
uintptr_t Offsets::GetForceAttack2()
{
	return sdk->ClientBase + m_dwForceAttack2;
}

// Get address for force forward
uintptr_t Offsets::GetForceForward()
{
	return sdk->ClientBase + m_dwForceForward;
}

// Get address for force back
uintptr_t Offsets::GetForceBack()
{
	return sdk->ClientBase + m_dwForceBack;
}

// Get address for angles
uintptr_t Offsets::GetVecAngles()
{
	return sdk->EngineBase + dwVecAngles;
}

// Initiate player structure offsets from signatures
PlayerOffsets::PlayerOffsets(SDK* sdk)
{
	this->sdk = sdk;

	// Health offset
	m_iHealth = sdk->GetClientOffsets((PBYTE)"\x83\xB9\x00\x00\x00\x00\x00\x7F\x2C", "xx?xxxxxx", 2);

	// Team number offset
	m_iTeamNum = sdk->GetClientOffsets((PBYTE)"\xFF\xB1\x00\x00\x00\x00\xE8\x95\x75\x03\x00\x83\xC4\x04", "xx?xxxxxxxxxxx", 2);

	// Life state offset
	m_lifeState = sdk->GetClientOffsets((PBYTE)"\x80\xB9\x00\x00\x00\x00\x00\x0F\x94\xC0", "xx?xxxxxxx", 2);

	// Flags offset
	m_fFlags = sdk->GetClientOffsets((PBYTE)"\x8B\x98\x00\x00\x00\x00\xD1\xEB\x80\xE3\x01", "xx??xxxxxxx", 2);

	// Observer mode offset
	m_iObserverMode = sdk->GetClientOffsets((PBYTE)"\x8B\x86\x00\x00\x00\x00\x5E\xC3\x90\x49", "xx??xxxxxx", 2);

	// Vector origin offset
	m_vecOrigin = sdk->GetClientOffsets((PBYTE)"\xF3\x0F\x10\x86\x00\x00\x00\x00\x8B\xCE\x8B\x45\x00", "xxxx??xxxxxx?", 4);

	// FOV offset
	m_iFOV = sdk->GetClientOffsets((PBYTE)"\xF3\x0F\x11\x45\xFC\x8B\x86\x00\x00\x00\x00\x85\xC0", "xxxxxxx????xx", 7);

	// Vector view offset offset
	m_vecViewOffset = 0xFC;

	// Player class offset
	m_PlayerClass = 0xC20 - 1;

	// Vector view angles offset
	// m_angRotation = sdk->GetClientOffsets((PBYTE)"\xD9\x9E\x00\x00\x00\x00\xF3\x0F\x10\x86\x00\x00\x00\x00\x0F\x2E\x86", "xx????xxxxx????xxx", 11);
	m_angRotation = 0x370;

	// Glow enabled offset
	m_bGlowEnabled = 0xDBC;

	// Dormant offset
	m_bDormant = 0x1AA;

	// Cross hair ID offset
	m_iCrosshairID = sdk->GetClientOffsets((PBYTE)"\x8B\xC1\xC3\xCC\x8B\x81\x00\x00\x00\x00\xC3", "xxxxxx????x", 6);

	// Bone matrix offset
	m_iBoneMatrix = sdk->GetClientOffsets((PBYTE)"\x8B\x83\x00\x00\x00\x00\x8B\xCB\x8B\x13", "xx??xxxxxx", 2);
}