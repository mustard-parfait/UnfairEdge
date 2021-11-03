#include "Include.h"

LocalPlayer::LocalPlayer(SDK* sdk, uintptr_t playerPointer, PlayerOffsets* offsets, Offsets* eOffsets)
{
	// Set player address and offsets for local player
	this->sdk = sdk;
	localPlayerEntity = playerPointer;
	pOffsets = offsets;
	this->eOffsets = eOffsets;

	// Set initial local player health
	m_iHealth = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iHealth), &m_iHealth, sizeof(m_iHealth), 0);

	// Set initial local player team number
	m_iTeamNum = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iTeamNum), &m_iTeamNum, sizeof(m_iTeamNum), 0);

	// Set intial local player cross hair ID
	m_iCrosshairID = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iCrosshairID), &m_iCrosshairID, sizeof(m_iCrosshairID), 0);

	// Set initial local player flags
	m_fFlags = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_fFlags), &m_fFlags, sizeof(m_fFlags), 0);

	// Set initial local player vector origin
	m_vecOrigin = {};
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_vecOrigin), &m_vecOrigin, sizeof(m_vecOrigin), 0);

	// Set initial local player vector view offset
	m_vecViewOffset = {};
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_vecViewOffset), &m_vecViewOffset, sizeof(m_vecViewOffset), 0);

	// Set initial local player vector head origin
	m_vecHeadOrigin = GetHeadOrigin();

	// Set initial local player angles
	m_angRotation = {};
	ReadProcessMemory(sdk->hProc, (BYTE*)(eOffsets->GetVecAngles()), &m_angRotation, sizeof(m_angRotation), 0);
}

// Get local player health
int LocalPlayer::GetHealth()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iHealth), &m_iHealth, sizeof(m_iHealth), 0);
	return m_iHealth;
}

// Get local player team number
int LocalPlayer::GetTeamNum()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iTeamNum), &m_iTeamNum, sizeof(m_iTeamNum), 0);
	return m_iTeamNum;
}

// Get local player cross hair ID
int LocalPlayer::GetCrosshairID()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_iCrosshairID), &m_iCrosshairID, sizeof(m_iCrosshairID), 0);
	return m_iCrosshairID;
}

// Get local player flags
BYTE LocalPlayer::GetFlags()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_fFlags), &m_fFlags, sizeof(m_fFlags), 0);
	return m_fFlags;
}

// Get local player origin
Vec3 LocalPlayer::GetOrigin()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_vecOrigin), &m_vecOrigin, sizeof(m_vecOrigin), 0);
	return m_vecOrigin;
}

// Get local player view offset
Vec3 LocalPlayer::GetViewOffset()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(localPlayerEntity + pOffsets->m_vecViewOffset), &m_vecViewOffset, sizeof(m_vecViewOffset), 0);
	return m_vecViewOffset;
}

// Get local player head origin
Vec3 LocalPlayer::GetHeadOrigin()
{
	Vec3 HeadOrigin;
	
	m_vecOrigin = GetOrigin();
	m_vecViewOffset = GetViewOffset();

	HeadOrigin.x = m_vecOrigin.x + m_vecViewOffset.x;
	HeadOrigin.y = m_vecOrigin.y + m_vecViewOffset.y;
	HeadOrigin.z = m_vecOrigin.z + m_vecViewOffset.z;

	return HeadOrigin;
}

// Get local player angles
Vec3 LocalPlayer::GetAngles()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(eOffsets->GetVecAngles()), &m_angRotation, sizeof(m_angRotation), 0);
	return m_angRotation;
}

// Write to local player angles
void LocalPlayer::WriteAngles(float x, float y)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetVecAngles(), &x, sizeof(x), 0);
	WriteProcessMemory(sdk->hProc, (BYTE*)(eOffsets->GetVecAngles() + 4), &y, sizeof(y), 0);
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
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceJump(), &value, sizeof(value), 0);
}

// Force duck local player
void LocalPlayer::ForceDuck(int value)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceDuck(), &value, sizeof(value), 0);
}

// Force attack (left) local player
void LocalPlayer::ForceAttack1(int value)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceAttack1(), &value, sizeof(value), 0);
}

// Force attack (right) local player
void LocalPlayer::ForceAttack2(int value)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceAttack2(), &value, sizeof(value), 0);
}

// Force forward local player
void LocalPlayer::ForceForward(int value)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceForward(), &value, sizeof(value), 0);
}

// Force back local player
void LocalPlayer::ForceBack(int value)
{
	WriteProcessMemory(sdk->hProc, (BYTE*)eOffsets->GetForceBack(), &value, sizeof(value), 0);
}

PlayerEntity::PlayerEntity(SDK* sdk, uintptr_t playerPointer, PlayerOffsets* offsets)
{
	// Set player address and offsets for player entity
	this->sdk = sdk;
	this->playerEntity = playerPointer;
	pOffsets = offsets;

	// Set initial player entity health
	m_iHealth = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iHealth), &m_iHealth, sizeof(m_iHealth), 0);

	// Set initial player entity team number
	m_iTeamNum = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iTeamNum), &m_iTeamNum, sizeof(m_iTeamNum), 0);

	// Set initial player entity dormant
	m_bDormant = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_bDormant), &m_bDormant, sizeof(m_bDormant), 0);

	// Set initial player entity bone matrix value
	m_iBoneMatrix = 0;
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iBoneMatrix), &m_iBoneMatrix, sizeof(m_iBoneMatrix), 0);

	// Set initial player entity origin
	m_vecOrigin = {};
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_vecOrigin), &m_vecOrigin, sizeof(m_vecOrigin), 0);

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
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iHealth), &m_iHealth, sizeof(m_iHealth), 0);
	return m_iHealth;
}

// Get player entity team number
int PlayerEntity::GetTeamNum()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iTeamNum), &m_iTeamNum, sizeof(m_iTeamNum), 0);
	return m_iTeamNum;
}

// Get player entity dormant
bool PlayerEntity::GetDormant()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_bDormant), &m_bDormant, sizeof(m_bDormant), 0);
	return m_bDormant;
}

// Get player entity bone matrix
int PlayerEntity::GetBoneMatrix()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_iBoneMatrix), &m_iBoneMatrix, sizeof(m_iBoneMatrix), 0);
	return m_iBoneMatrix;
}

// Get player entity origin
Vec3 PlayerEntity::GetPlayerOrigin()
{
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_vecOrigin), &m_vecOrigin, sizeof(m_vecOrigin), 0);
	return m_vecOrigin;
}

// Get player entity class
char* PlayerEntity::GetPlayerClass()
{
	char PlayerClass[40];
	ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity + pOffsets->m_PlayerClass), &PlayerClass, sizeof(PlayerClass), 0);
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
Vec3 PlayerEntity::GetVecBoneMatrix()
{
	Vec3 BoneMatrix;

	ReadProcessMemory(sdk->hProc, (BYTE*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x0C), (BYTE*)&BoneMatrix.x, sizeof(BoneMatrix.x), 0);
	ReadProcessMemory(sdk->hProc, (BYTE*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x1C), (BYTE*)&BoneMatrix.y, sizeof(BoneMatrix.y), 0);
	ReadProcessMemory(sdk->hProc, (BYTE*)(m_iBoneMatrix + m_iBoneID * 0x30 + 0x2C), (BYTE*)&BoneMatrix.z, sizeof(BoneMatrix.z), 0);

	return BoneMatrix;
}