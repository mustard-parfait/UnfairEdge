#include "Include.h"

void Aimbot(SDK* sdk, uintptr_t* currentFocusPlayer, Offsets* offsets, PlayerOffsets* pOffsets)
{
	// Initiate local player
	LocalPlayer* lpPlayer = new LocalPlayer(sdk, offsets->GetLocalPlayer(), pOffsets, offsets);
	if (lpPlayer == NULL) return;

    // Set current focus player if not already
    if (*currentFocusPlayer == NULL)
    {
        for (DWORD i = 0; i <= 24; i++)
        {
            uintptr_t PlayerEntityPointer = offsets->GetPlayerEntity(i);

            if (PlayerEntityPointer != 0)
            {
                PlayerEntity* playerEntity = new PlayerEntity(sdk, PlayerEntityPointer, pOffsets);

                if (playerEntity->m_iHealth >= 1 && PlayerEntityPointer != *currentFocusPlayer && playerEntity->playerEntity != lpPlayer->localPlayerEntity && playerEntity->m_iTeamNum != lpPlayer->m_iTeamNum)
                {
                    if (!playerEntity->m_bDormant)
                    {
                        *currentFocusPlayer = PlayerEntityPointer;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // Initiate current focus player
        PlayerEntity* CurrentPlayerEntity = new PlayerEntity(sdk, *currentFocusPlayer, pOffsets);

        // Check for enemy health and change current player
        if (CurrentPlayerEntity->m_iHealth < 1 || GetAsyncKeyState(VK_NUMPAD2))
        {
            *currentFocusPlayer = 0;
            return;
        }

        // Calculate 3D distance
        Vec3* dist = new Vec3();
        Vec3 playerOrigin = lpPlayer->GetHeadOrigin();
        Vec3 enemyOrigin = CurrentPlayerEntity->GetVecBoneMatrix();

        dist->x = enemyOrigin.x - playerOrigin.x;
        dist->y = enemyOrigin.y - playerOrigin.y;
        dist->z = enemyOrigin.z - playerOrigin.z;

        // Calculate magnitude
        float magnitude = sqrt(dist->x * dist->x + dist->y * dist->y + dist->z * dist->z);

        // Calculate pitch and yaw angles
        float m_dwAnglePitch = -asin(dist->z / magnitude) * (180 / M_PI);
        float m_dwAngleYaw = atan2(dist->y, dist->x) * (180 / M_PI);
        if (m_dwAnglePitch >= -89 && m_dwAnglePitch <= 89 && m_dwAngleYaw >= -180 && m_dwAngleYaw <= 180)
            lpPlayer->WriteAngles(m_dwAnglePitch, m_dwAngleYaw);
    }
}

// Trigger bot using force controllabes
void Triggerbot(SDK* sdk, Offsets* offsets, PlayerOffsets* pOffsets)
{
    LocalPlayer* lpPlayer = new LocalPlayer(sdk, offsets->GetLocalPlayer(), pOffsets, offsets);
    if (lpPlayer == NULL) return;

    // Check for cross hair and shoot
    if (lpPlayer->GetCrosshairID() == 4)
        lpPlayer->ForceAttack1(6);
}

// Rocket Jump using force controllables
void RocketJump(SDK* sdk, Offsets* offsets, PlayerOffsets* pOffsets, int jump)
{
    printf("TFI: Rocket Jump\n");

    // Get local player entity
    LocalPlayer* lpPlayer = new LocalPlayer(sdk, offsets->GetLocalPlayer(), pOffsets, offsets);
    if (lpPlayer == NULL) return;

    // Get previous player angles
    Vec3 preAngles = lpPlayer->GetAngles();

    // Rocket Jump (only if on ground)
    if (lpPlayer->GetFlags() & (1 << 0))
    {
        if (jump == 1)
            lpPlayer->WriteAngles(89.f, 0.f);
        else if (jump == 2)
            lpPlayer->WriteAngles(50.f, preAngles.y - 180);
        else if (jump == 3)
            lpPlayer->WriteAngles(45.f, preAngles.y - 90);

        lpPlayer->ForceDuck(5);
        Sleep(2);
        lpPlayer->ForceJump(6);
        Sleep(7);
        lpPlayer->ForceAttack1(6);
        lpPlayer->ForceDuck(4);
        Sleep(15);
        lpPlayer->WriteAngles(preAngles.x, preAngles.y);
    }
}

// Glow object (player entities (works after a respawn))
void GlowObject(SDK* sdk, Offsets* offsets, PlayerOffsets* pOffsets)
{
    // Initiate player offsets and local player
    LocalPlayer* lpPlayer = new LocalPlayer(sdk, offsets->GetLocalPlayer(), pOffsets, offsets);
    if (lpPlayer == NULL) return;

    for (DWORD i = 2; i < 24; i++)
    {
        uintptr_t playerEntityPointer = offsets->GetPlayerEntity(i);

        // Check if address if exists
        if (playerEntityPointer != 0)
        {
            // Initiate player entity
            PlayerEntity* playerEntity = new PlayerEntity(sdk, playerEntityPointer, pOffsets);
            if (playerEntity == NULL) continue;
            BYTE EnableGlow = 1;

            // If player is enemy and nearby, set glow to true (only if DBC is set to 0)
            if (playerEntity->m_iTeamNum != lpPlayer->m_iTeamNum && !playerEntity->m_bDormant)
            {
                BYTE GlowEnabled = 0;
                ReadProcessMemory(sdk->hProc, (BYTE*)(playerEntity->playerEntity + pOffsets->m_bGlowEnabled), &GlowEnabled, sizeof(GlowEnabled), 0);
                if (!GlowEnabled)
                {
                    WriteProcessMemory(sdk->hProc, (BYTE*)(playerEntity->playerEntity + pOffsets->m_bGlowEnabled), &EnableGlow, sizeof(EnableGlow), 0);
                    WriteProcessMemory(sdk->hProc, (BYTE*)(playerEntity->playerEntity + pOffsets->m_bGlowEnabled + 1), &EnableGlow, sizeof(EnableGlow), 0);
                }
            }
        }
    }
}

// Bunny hop based if local player is on ground
void BunnyHop(SDK* sdk, Offsets* offsets, PlayerOffsets* pOffsets)
{
    // Initiate player offsets and local player
    LocalPlayer* lpPlayer = new LocalPlayer(sdk, offsets->GetLocalPlayer(), pOffsets, offsets);
    if (lpPlayer == NULL) return;

    // Force duck and jump if ON_GROUND
    if ((lpPlayer->m_fFlags) & (1 << 0) & GetAsyncKeyState(VK_SPACE))
    {
        lpPlayer->ForceJump(6);
    }
}