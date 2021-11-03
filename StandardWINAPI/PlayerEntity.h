#pragma once

// Define local player (additional force controls)
class LocalPlayer
{
public:
	SDK* sdk;
	uintptr_t localPlayerEntity;
	PlayerOffsets* pOffsets;
	Offsets* eOffsets;
	int m_iHealth;
	int m_iTeamNum;
	int m_iCrosshairID;
	BYTE m_fFlags;
	Vec3 m_vecOrigin;
	Vec3 m_vecViewOffset;
	Vec3 m_vecHeadOrigin;
	Vec3 m_angRotation;

	LocalPlayer(SDK* sdk, uintptr_t playerPointer, PlayerOffsets* offsets, Offsets* eOffsets);

	// Get player attributes
	int GetHealth();
	int GetTeamNum();
	int GetCrosshairID();
	BYTE GetFlags();
	Vec3 GetOrigin();
	Vec3 GetViewOffset();
	Vec3 GetAngles();
	Vec3 GetHeadOrigin();
	void CheckAngles(float& m_dwAnglePitch, float& m_dwAngleYaw);

	// Set player attributes
	void WriteAngles(float x, float y);
	void ForceJump(int value);
	void ForceDuck(int value);
	void ForceAttack1(int value);
	void ForceAttack2(int value);
	void ForceForward(int value);
	void ForceBack(int value);
};

class PlayerEntity
{
public:
	SDK* sdk;
	uintptr_t playerEntity;
	PlayerOffsets* pOffsets;
	int m_iHealth;
	int m_iTeamNum;
	bool m_bDormant;
	int m_iBoneID;
	int m_iBoneMatrix;
	Vec3 m_vecBoneMatrix;
	Vec3 m_vecOrigin;
	char* m_PlayerClass;

	const char* scout = "scout/phonemes/phonemes_strong";
	const char* soldier = "soldier/phonemes/phonemes_strong";
	const char* s_strong = "s_strong";
	const char* demo = "demo/phonemes/phonemes_strong";
	const char* heavy = "heavy/phonemes/phonemes_strong";
	const char* medic = "medic/phonemes/phonemes_strong";
	const char* engineer = "engineer/phonemes/phonemes_strong";
	const char* spy = "spy/phonemes/phonemes_strong";

	PlayerEntity(SDK* sdk, uintptr_t playerPointer, PlayerOffsets* offsets);
	int GetHealth();
	int GetTeamNum();
	bool GetDormant();
	int GetBoneMatrix();
	int GetBoneID();
	char* GetPlayerClass();
	Vec3 GetVecBoneMatrix();
	Vec3 GetPlayerOrigin();
};

