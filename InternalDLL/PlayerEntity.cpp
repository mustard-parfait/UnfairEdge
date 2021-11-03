#include "pch.h"
#include "Include.h"

LocalPlayer::LocalPlayer(uintptr_t playerPointer, PlayerOffsets* offsets)
{
	// Set player address and offsets for local player
	this->localPlayerEntity = playerPointer;
	this->offsets = offsets;

	// Set initial local player health
	m_iHealth = *(int*)(localPlayerEntity + offsets->m_iHealth);

	// Set initial local player team number
	m_iTeamNum = *(int*)(localPlayerEntity + offsets->m_iTeamNum);
	
	// Set initial local player cross hair ID
	m_iCrosshairID = *(int*)(localPlayerEntity + offsets->m_iCrosshairID);

	// Set initial local player flags
	m_fFlags = *(BYTE*)(localPlayerEntity + offsets->m_fFlags);

	// Set initial local player vector origin
	m_vecOrigin = (Vec3*)(localPlayerEntity + offsets->m_vecOrigin);

	// Set initial local player vector view offset
	m_vecViewOffset = (Vec3*)(localPlayerEntity + offsets->m_vecViewOffset);

	// Set initial local player vector head origin
	m_vecHeadOrigin = GetHeadOrigin();

	// Set initial local player angles
	uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	m_angRotation = (Vec3*)(engineBase + 0x469494);

	// Set initial local player view matrix
	memcpy(&m_frViewMatrix, (PBYTE*)(engineBase + 0x59FB80), sizeof(m_frViewMatrix));

	// Set local player controllables address
	uintptr_t clientBase = (uintptr_t)GetModuleHandle(L"client.dll");
	m_dwForceJump = clientBase + 0xC6C320;
	m_dwForceDuck = clientBase + 0xC6C35C;
	m_dwForceAttack1 = clientBase + 0xC6C32C;
	m_dwForceAttack2 = clientBase + 0xC6C338;
	m_dwForceForward = clientBase + 0xC6B334;
	m_dwForceBack = clientBase + 0xC6B340;
	m_dwForceLeft = clientBase + 0xC6B2C8;
	m_dwForceRight = clientBase + 0xC6B2D4;
}

// Get local player health
int LocalPlayer::GetHealth()
{
	return *(int*)(localPlayerEntity + offsets->m_iHealth);
}

// Get local player team number
int LocalPlayer::GetTeamNum()
{
	return *(int*)(localPlayerEntity + offsets->m_iTeamNum);
}

// Get local player cross hair ID
int LocalPlayer::GetCrosshairID()
{
	return *(int*)(localPlayerEntity + offsets->m_iCrosshairID);
}

// Get local player flags
BYTE LocalPlayer::GetFlags()
{
	return *(BYTE*)(localPlayerEntity + offsets->m_fFlags);
}

// Get local player origin
Vec3* LocalPlayer::GetOrigin()
{
	return (Vec3*)(localPlayerEntity + offsets->m_vecOrigin);
}

// Get local player view offset
Vec3* LocalPlayer::GetViewOffset()
{
	return (Vec3*)(localPlayerEntity + offsets->m_vecViewOffset);
}

// Get local player angles
Vec3* LocalPlayer::GetAngles()
{
	uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	return (Vec3*)(engineBase + 0x469494);
}

// Get local player head origin
Vec3* LocalPlayer::GetHeadOrigin()
{
	Vec3 HeadOrigin;

	HeadOrigin.x = m_vecOrigin->x + m_vecViewOffset->x;
	HeadOrigin.y = m_vecOrigin->y + m_vecViewOffset->y;
	HeadOrigin.z = m_vecOrigin->z + m_vecViewOffset->z;

	return &HeadOrigin;
}

// Set local player angles
void LocalPlayer::SetAngles(float m_dwAnglePitch, float m_dwAngleYaw)
{
	m_angRotation->x = m_dwAnglePitch;
	m_angRotation->y = m_dwAngleYaw;
}

// Write to local player angles
void LocalPlayer::WriteAngles(float x, float y)
{
	uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	memcpy((BYTE*)(engineBase + 0x469494), &x, sizeof(x));
	memcpy((BYTE*)(engineBase + 0x469498), &y, sizeof(y));
}

// Get local player view matrix
float* LocalPlayer::GetViewMatrix()
{
	uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	memcpy(&m_frViewMatrix, (PBYTE*)(engineBase + 0x59FB80), sizeof(m_frViewMatrix));
	return m_frViewMatrix;
}

// Check and set angles of local player
void LocalPlayer::CheckAngles(float& m_dwAnglePitch, float& m_dwAngleYaw)
{
	if (m_dwAnglePitch > 89.f && m_dwAnglePitch <= 180.f)
		m_dwAnglePitch = 89.f;
	if (m_dwAnglePitch >= 180.f)
		m_dwAnglePitch -= 360.f;
	if (m_dwAnglePitch < -89.0f)
		m_dwAnglePitch = -89.0f;
	if (m_dwAngleYaw > 180.0f)
		m_dwAngleYaw -= 360.0f;
	if (m_dwAngleYaw < -180.0f)
		m_dwAngleYaw += 360.0f;
}

// Force jump local player
void LocalPlayer::ForceJump(int value)
{
	*(int*)m_dwForceJump = value;
}

// Force duck local player
void LocalPlayer::ForceDuck(int value)
{
	*(int*)m_dwForceDuck = value;
}

// Force attack (left) local player
void LocalPlayer::ForceAttack1(int value)
{
	*(int*)m_dwForceAttack1 = value;
}

// Force attack (right) local player
void LocalPlayer::ForceAttack2(int value)
{
	*(int*)m_dwForceAttack2 = value;
}

// Force forward local player
void LocalPlayer::ForceForward(int value)
{
	*(int*)m_dwForceForward = value;
}

// Force back local player
void LocalPlayer::ForceBack(int value)
{
	*(int*)m_dwForceBack = value;
}

// Force left local player
void LocalPlayer::ForceLeft(int value)
{
	*(int*)m_dwForceLeft = value;
}

// Force right local player
void LocalPlayer::ForceRight(int value)
{
	*(int*)m_dwForceRight = value;
}

PlayerEntity::PlayerEntity(uintptr_t playerPointer, PlayerOffsets* offsets)
{
	// Set player address and offsets for player entity
	this->playerEntity = playerPointer;
	this->offsets = offsets;

	// Set initial player entity health
	m_iHealth = *(int*)(playerEntity + offsets->m_iHealth);

	// Set initial player entity team number
	m_iTeamNum = *(int*)(playerEntity + offsets->m_iTeamNum);

	// Set initial player entity dormant
	m_bDormant = *(bool*)(playerEntity + offsets->m_bDormant);

	// Set initial player entity bone matrix value
	m_iBoneMatrix = *(int*)(playerEntity + offsets->m_iBoneMatrix);

	// Set initial player entity origin
	m_vecOrigin = (Vec3*)(playerEntity + offsets->m_vecOrigin);

	// Set initial player entity class
	m_PlayerClass = GetPlayerClass();

	// Set initial player entity bone ID
	m_iBoneID = GetBoneID();

	// Set initial player entity vector bone matrix
	m_vecBoneMatrix = GetVecBoneMatrix();
}

// Get player entity health
int PlayerEntity::GetHealth()
{
	return *(int*)(playerEntity + offsets->m_iHealth);
}

// Get player entity team number
int PlayerEntity::GetTeamNum()
{
	return *(int*)(playerEntity + offsets->m_iTeamNum);
}

// Get player entity dormant
bool PlayerEntity::GetDormant()
{
	return *(bool*)(playerEntity + offsets->m_bDormant);
}

// Get player entity bone matrix
int PlayerEntity::GetBoneMatrix()
{
	return *(int*)(playerEntity + offsets->m_iBoneMatrix);
}

// Get player entity origin
Vec3* PlayerEntity::GetPlayerOrigin()
{
	return (Vec3*)(playerEntity + offsets->m_vecOrigin);
}

// Get player entity class
char* PlayerEntity::GetPlayerClass()
{
	char PlayerClass[40];

	for (DWORD i = 0; i < sizeof(PlayerClass) / sizeof(char); i++)
	{
		PlayerClass[i] = *(char*)(playerEntity + offsets->m_PlayerClass + i);
	}

	return PlayerClass;
}

// Get player entity bone ID
int PlayerEntity::GetBoneID()
{
	if (!strcmp(m_PlayerClass, scout))
	{
		return 6;
	}
	else if (!strcmp(m_PlayerClass, soldier))
	{
		return 6;
	}
	else if (!strcmp(m_PlayerClass, s_strong))
	{
		return 6;
	}
	else if (!strcmp(m_PlayerClass, demo))
	{
		return 16;
	}
	else if (!strcmp(m_PlayerClass, heavy))
	{
		return 6;
	}
	else if (!strcmp(m_PlayerClass, medic))
	{
		return 6;
	}
	else if (!strcmp(m_PlayerClass, engineer))
	{
		return 8;
	}
	else if (!strcmp(m_PlayerClass, spy))
	{
		return 6;
	}
	else
	{
		return 0;
	}
}

// Get player entity bone matrix (player entity head origin)
Vec3* PlayerEntity::GetVecBoneMatrix()
{
	Vec3 BoneMatrix;
	
	BoneMatrix.x = *(float*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x0C);
	BoneMatrix.y = *(float*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x1C);
	BoneMatrix.z = *(float*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x2C);

	return &BoneMatrix;
}