#include "pch.h"
#include "Include.h"

// Compare RecvTable with required Table for NetVar
uintptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName)
{
	for (int i = 0; i < table->m_nProps; i++)
	{
		RecvProp CurrentProp = table->m_pProps[i];

		if (!strcmp(CurrentProp.m_pVarName, netvarName))
			return CurrentProp.m_Offset;

		if (CurrentProp.m_pDataTable)
		{
			uintptr_t offset = GetOffset(CurrentProp.m_pDataTable, tableName, netvarName);

			if (offset)
				return offset + CurrentProp.m_Offset;
		}
	}
	return 0;
}

// Get Offset for mentioned NetVarName, using ClientClass (comparing table name with each networked table)
uintptr_t GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* ClientClassPtr)
{
	ClientClass* CurrentClass = ClientClassPtr;

	while (CurrentClass)
	{
		if (!strcmp(tableName, CurrentClass->m_pRecvTable->m_pNetTableName))
			return GetOffset(CurrentClass->m_pRecvTable, tableName, netvarName);
		CurrentClass = CurrentClass->m_pNext;
	}

	return 0;
}

// Returns offsets from DT_BasePlayer class
DWORD PlayerOffsets::GetBasePlayerOffset(const char* netvarName)
{
	return GetNetVarOffset("DT_BasePlayer", netvarName, ClientClass);
}

// Returns offsets from DT_BaseEntity class
DWORD PlayerOffsets::GetBaseEntityOffset(const char* netvarName)
{
	return GetNetVarOffset("DT_BaseEntity", netvarName, ClientClass);
}

// Returns offsets from DT_BaseCombatCharacter class
DWORD PlayerOffsets::GetBaseCombatCharacter(const char* netvarName)
{
	return GetNetVarOffset("DT_BaseCombatCharacter", netvarName, ClientClass);
}

// Returns offsets from DT_TFWeaponKnife class
DWORD PlayerOffsets::GetTFWeaponKnife(const char* netvarName)
{
	return GetNetVarOffset("DT_TFWeaponKnife", netvarName, ClientClass);
}

// Initialize all player offsets
PlayerOffsets::PlayerOffsets()
{ 
	SDK* sdk = new SDK();
	ClientClass = sdk->GetClientClass();

	m_iHealth = GetBasePlayerOffset("m_iHealth");
	m_iTeamNum = GetBasePlayerOffset("m_iTeamNum");
	m_lifeState = GetBasePlayerOffset("m_lifeState");
	m_fFlags = GetBasePlayerOffset("m_fFlags");
	m_iObserverMode = GetBasePlayerOffset("m_iObserverMode");
	m_iFOV = GetBasePlayerOffset("m_iFOV");
	m_iDefaultFOV = GetBasePlayerOffset("m_iDefaultFOV");
	m_flMaxSpeed = GetBasePlayerOffset("m_flMaxSpeed");

	m_vecOrigin = GetBaseEntityOffset("m_vecOrigin");
	m_angRotation = GetBaseEntityOffset("m_angRotation");

	m_hMyWeapons = GetBaseCombatCharacter("m_hMyWeapons");
	m_hMyActiveWeapons = GetBaseCombatCharacter("m_hMyActiveWeapons");
	m_bGlowEnabled = GetBaseCombatCharacter("m_bGlowEnabled");

	m_bReadyToBackstab = GetTFWeaponKnife("m_bReadyToBackstab");

	m_vecViewOffset = 0xFC;
	m_bDormant = 0x1AA;
	m_iBoneMatrix = 0x5B0;
	m_PlayerClass = 0xC20 - 1;
	m_iCrosshairID = 0x177C;
}

class Ent
{
public:
	char pad_0000[164]; //0x0000
	uint32_t lifestate; //0x00A4
	uint32_t health; //0x00A8
	char pad_00AC[4]; //0x00AC
	uint32_t teamnum; //0x00B0
	char pad_00B4[72]; //0x00B4
	Vec3 viewoffset; //0x00FC
	char pad_0108[24]; //0x0108
	Vec3 velocity; //0x0120
	char pad_012C[124]; //0x012C
	uint16_t N000000B5; //0x01A8
	bool dormant; //0x01AA
	char pad_01AB[225]; //0x01AB
	Vec3 playercoords; //0x028C
	char pad_0298[228]; //0x0298
	uint32_t flags; //0x037C
	char pad_0380[560]; //0x0380
	class bones* boneMat; //0x05B0
	char pad_05B4[2688]; //0x05B4
	uint32_t fov; //0x1034
	char pad_1038[100]; //0x1038
	uint32_t observermode; //0x109C
	char pad_10A0[1756]; //0x10A0
	uint32_t crosshairid; //0x177C
	char pad_1780[604]; //0x1780
	uint32_t classID; //0x19DC
	char pad_19E0[5572]; //0x19E0
	Vec3 bodypartsscale; //0x2FA4
}; //Size: 0x31E8

class bones
{
public:
	char pad_0000[60]; //0x0000
	float chestBoneX; //0x003C
	char pad_0040[12]; //0x0040
	float chestBoneY; //0x004C
	char pad_0050[12]; //0x0050
	float chestBoneZ; //0x005C
	char pad_0060[204]; //0x0060
	float headPosX; //0x012C
	char pad_0130[12]; //0x0130
	float headPosY; //0x013C
	char pad_0140[12]; //0x0140
	float headPosZ; //0x014C
	char pad_0150[60]; //0x0150
	float engiHeadX; //0x018C
	char pad_0190[12]; //0x0190
	float engiHeadY; //0x019C
	char pad_01A0[12]; //0x01A0
	float engiHeadZ; //0x01AC
	char pad_01B0[348]; //0x01B0
	float demoHeadX; //0x030C
	char pad_0310[12]; //0x0310
	float demoHeadY; //0x031C
	char pad_0320[12]; //0x0320
	float demoHeadZ; //0x032C
	char pad_0330[276]; //0x0330
}; //Size: 0x0444
