#pragma once

class PlayerOffsets
{
public:
	ClientClass* ClientClass;
	DWORD m_iHealth;
	DWORD m_iTeamNum;
	DWORD m_lifeState;
	DWORD m_fFlags;
	DWORD m_iObserverMode;
	DWORD m_iFOV;
	DWORD m_iDefaultFOV;
	DWORD m_flMaxSpeed;
	DWORD m_vecOrigin;
	DWORD m_angRotation;
	DWORD m_hMyWeapons;
	DWORD m_hMyActiveWeapons;
	DWORD m_bGlowEnabled;
	DWORD m_vecViewOffset;
	DWORD m_bDormant;
	DWORD m_iBoneMatrix;
	DWORD m_PlayerClass;
	DWORD m_iCrosshairID;
	DWORD m_bReadyToBackstab;

	PlayerOffsets();

	DWORD GetBasePlayerOffset(const char* netvarName);
	DWORD GetBaseEntityOffset(const char* netvarName);
	DWORD GetBaseCombatCharacter(const char* netvarName);
	DWORD GetTFWeaponKnife(const char* netvarName);
};

uintptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName);
uintptr_t GetNetVarOffset(const char* tableName, const char* interfaceName, ClientClass* ClientClassPtr);
