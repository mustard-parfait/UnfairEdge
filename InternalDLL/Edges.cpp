#include "pch.h"
#include "Include.h"

// Aimbot using CreateMove to change angles
void* Aimbot(SDK* sdk, uintptr_t* currentFocusPlayer, bool* hooked)
{
    PlayerOffsets* pOffsets = new PlayerOffsets();
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);

    // Set current focus player if not already
    if (*currentFocusPlayer == NULL)
    {
        for (DWORD i = 2; i <= 24; i++)
        {
            uintptr_t PlayerEntityPointer = sdk->GetPlayerEntity(i);
            
            if (PlayerEntityPointer != 0)
            {
                PlayerEntity* playerEntity = new PlayerEntity(PlayerEntityPointer, pOffsets);

                if (playerEntity->m_iHealth >= 1 && PlayerEntityPointer != *currentFocusPlayer && playerEntity->m_iTeamNum != lpPlayer->m_iTeamNum)
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
        PlayerEntity* CurrentPlayerEntity = new PlayerEntity(*currentFocusPlayer, pOffsets);

        // Check for enemy health and change current player
        if (CurrentPlayerEntity->m_iHealth < 1 || GetAsyncKeyState(VK_NUMPAD2))
        {
            *currentFocusPlayer = 0;
            return nullptr;
        }

        // Calculate 3D distance
        Vec3* dist = new Vec3();

        Vec3* playerOrigin = lpPlayer->GetHeadOrigin();
        float playerOriginX = playerOrigin->x;
        float playerOriginY = playerOrigin->y;
        float playerOriginZ = playerOrigin->z;

        Vec3* enemyOrigin = CurrentPlayerEntity->GetVecBoneMatrix();
        float enemyOriginX = enemyOrigin->x;
        float enemyOriginY = enemyOrigin->y;
        float enemyOriginZ = enemyOrigin->z;

        dist->x = enemyOriginX - playerOriginX;
        dist->y = enemyOriginY - playerOriginY;
        dist->z = enemyOriginZ - playerOriginZ;

        // Calculate magnitude
        float magnitude = sqrt(dist->x * dist->x + dist->y * dist->y + dist->z * dist->z);

        // Calculate pitch and yaw angles
        float m_dwAnglePitch = -asin(dist->z / magnitude) * (180 / M_PI);
        float m_dwAngleYaw = atan2(dist->y, dist->x) * (180 / M_PI);
        lpPlayer->CheckAngles(m_dwAnglePitch, m_dwAngleYaw);

        // Define hook class
        lpPlayer->SetAngles(m_dwAnglePitch, m_dwAngleYaw);
        Hooking* hook = new Hooking(lpPlayer);

        // Hook CreateMove if not already
        if (!hooked)
        {
            hook->oCreateMove = (fCreateMove)hook->TrampolineHook((char*)sdk->CreateMoveAddress, (char*)fakeCreateMove, hook->hookLength);
            if (hooked != NULL) *hooked = true;
        }

        return hook;
    }
    return nullptr;
}

// Trigger bot using force controllabes
void Triggerbot(SDK* sdk)
{
    PlayerOffsets* pOffsets = new PlayerOffsets();
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);
    if (lpPlayer == NULL) return;

    // Check for cross hair and shoot
    if (lpPlayer->GetCrosshairID() == 4)
        lpPlayer->ForceAttack1(6);
}

// Rocket Jump using force controllables (try with CUserCmd later)
void RocketJump(SDK* sdk, int jump)
{
    printf("TFI: Rocket Jump\n");
    // Initialize player offsets
    PlayerOffsets* pOffsets = new PlayerOffsets();

    // Get local player entity
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);
    if (lpPlayer == NULL) return;

    // Get previous player angles
    Vec3 preAngles = *lpPlayer->GetAngles();

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
void GlowObject(SDK* sdk)
{
    // Initiate player offsets and local player
    PlayerOffsets* pOffsets = new PlayerOffsets();
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);
    if (lpPlayer == NULL) return;

    for (DWORD i = 2; i < 24; i++)
    {
        uintptr_t playerEntityPointer = sdk->GetPlayerEntity(i);

        // Check if address if exists
        if (playerEntityPointer != 0)
        {
            // Initiate player entity
            PlayerEntity* playerEntity = new PlayerEntity(playerEntityPointer, pOffsets);
            if (playerEntity == NULL) continue;

            // If player is enemy and nearby, set glow to true (only if DBC is set to 0)
            if (playerEntity->m_iTeamNum != lpPlayer->m_iTeamNum && !playerEntity->m_bDormant)
            {
                if (*(BYTE*)(playerEntity->playerEntity + 0xDBC) == 0)
                {
                    *(BYTE*)(playerEntity->playerEntity + 0xDBC) = 1;
                    *(BYTE*)(playerEntity->playerEntity + 0xDBD) = 1;
                }
            }
        }
    }
}

// Bunny hop based if local player is on ground
void BunnyHop(SDK* sdk)
{
    // Initiate player offsets and local player
    PlayerOffsets* pOffsets = new PlayerOffsets();
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);
    if (lpPlayer == NULL) return;

    // Force duck and jump if ON_GROUND
    if ((lpPlayer->m_fFlags) & (1 << 0) & GetAsyncKeyState(VK_SPACE))
    {
        lpPlayer->ForceJump(6);
    }
}

// Draw spawn lines around the players (green - friendly, red - enemy)
void* ogDirectXDevice[119];
BYTE DirectXScene[7]{ 0 };
fnDirectXScene oDirectXScene = nullptr;
extern LPDIRECT3DDEVICE9 lpDevice = nullptr;

// Fake DirectX scence
void APIENTRY fakeDirectXScene(LPDIRECT3DDEVICE9 ogDevice)
{
    if (!lpDevice)
        lpDevice = ogDevice;

    SDK* sdk = new SDK();
    PlayerOffsets* pOffsets = new PlayerOffsets();
    LocalPlayer* lpPlayer = new LocalPlayer(sdk->GetLocalPlayerEntity(), pOffsets);

    for (int i = 2; i < 24; i++)
    {
        uintptr_t PlayerEntityPointer = sdk->GetPlayerEntity(i);

        if (PlayerEntityPointer != 0)
        {
            PlayerEntity* player = new PlayerEntity(PlayerEntityPointer, pOffsets);

            if (!player->m_bDormant && player != NULL && player->playerEntity != lpPlayer->localPlayerEntity && player->m_iHealth >= 1)
            {
                D3DCOLOR color;
                if (player->m_iTeamNum == lpPlayer->m_iTeamNum)
                    color = D3DCOLOR_ARGB(255, 0, 255, 0);
                else
                    color = D3DCOLOR_ARGB(255, 255, 0, 0);

                Vec3 PlayerHead3D = *player->GetVecBoneMatrix();
                PlayerHead3D.z += player->m_iBoneID;
                Vec2 Player2D, PlayerHead2D;
                if (sdk->GameToScreen(*player->m_vecOrigin, Player2D))
                {
                    DrawLine(Player2D.x, Player2D.y, WindowWidth / 2, WindowHeight, 2, color);
                    if (sdk->GameToScreen(PlayerHead3D, PlayerHead2D))
                        DrawESPBox(Player2D, PlayerHead2D, 2, color);
                }
            }
        }
    }

    oDirectXScene(lpDevice);
}

// 
void* ESP(SDK* sdk)
{
    Hooking* hook = new Hooking();

    // Hook DirectX Device
    if (GetDirectXDevice(ogDirectXDevice, sizeof(ogDirectXDevice)))
    {
        memcpy(DirectXScene, (char*)ogDirectXDevice[42], hook->hookLength);
        oDirectXScene = (fnDirectXScene)hook->TrampolineHook((char*)ogDirectXDevice[42], (char*)fakeDirectXScene, hook->hookLength);
    }

    return hook;
}

// Patch fake DirectX Scene
void PatchDirectX(void* hook)
{
    ((Hooking*)hook)->Patch((BYTE*)ogDirectXDevice[42], DirectXScene, ((Hooking*)hook)->hookLength);
}