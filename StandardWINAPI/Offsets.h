#pragma once

// Class for offsets that hold static addresses
class Offsets
{
public:
	SDK* sdk;
	uintptr_t LocalPlayer;
	uintptr_t PlayerEntityList;
	uintptr_t PlayerControllables;
	uintptr_t m_dwForceJump;
	uintptr_t m_dwForceDuck;
	uintptr_t m_dwForceAttack1;
	uintptr_t m_dwForceAttack2;
	uintptr_t m_dwForceForward;
	uintptr_t m_dwForceBack;
	uintptr_t dwVecAngles;

	Offsets(SDK* sdk);
	uintptr_t GetLocalPlayer();
	uintptr_t GetPlayerEntityList();
	uintptr_t GetPlayerEntity(int num);
	uintptr_t GetForceJump();
	uintptr_t GetForceDuck();
	uintptr_t GetForceAttack1();
	uintptr_t GetForceAttack2();
	uintptr_t GetForceForward();
	uintptr_t GetForceBack();
	uintptr_t GetVecAngles();
};

// Class for offsets that hold player offsets
class PlayerOffsets
{
public:
	SDK* sdk;
	DWORD m_iHealth;
	DWORD m_iTeamNum;
	DWORD m_lifeState;
	DWORD m_fFlags;
	DWORD m_iObserverMode;
	DWORD m_iFOV;
	DWORD m_vecOrigin;
	DWORD m_angRotation;
	DWORD m_bGlowEnabled;
	DWORD m_vecViewOffset;
	DWORD m_bDormant;
	DWORD m_iBoneMatrix;
	DWORD m_PlayerClass;
	DWORD m_iCrosshairID;

	PlayerOffsets(SDK* sdk);
};