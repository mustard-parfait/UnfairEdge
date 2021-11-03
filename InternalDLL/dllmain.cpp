#include "pch.h"
#include "Include.h"

// Initial thread after attaching the dll
DWORD WINAPI ConThread(HMODULE hModule)
{
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    printf("TF2 Internal\n");

    // Vars for aimbot
    bool bAimbot = false;
    uintptr_t currentFocusPlayer = 0;
    bool hooked = false;
    Hooking* hook = nullptr;

    bool bGlow = false;
    bool bTriggerbot = false;
    bool bBunnyhop = false;
    bool bESP = false;
    void* hESP = nullptr;

    SDK* sdk = new SDK();

    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bAimbot = !bAimbot;
            if (bAimbot)
                printf("TFI: Activated Aim Bot\n");
            else
                printf("TFI: Deactivated Aim Bot\n");
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
            RocketJump(sdk, 1);
        
        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
            RocketJump(sdk, 2);

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
            RocketJump(sdk, 3);

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
            GlowObject(sdk);

        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            bBunnyhop = !bBunnyhop;
            if (bBunnyhop)
                printf("TFI: Activated Bunny Hop\n");
            else
                printf("TFI: Deactivated Bunny Hop\n");
        }

        if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        {
            bTriggerbot = !bTriggerbot;
            if (bTriggerbot)
                printf("TFI: Activated Trigger Bot\n");
            else
                printf("TFI: Deactivated Trigger Bot\n");
        }

        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            bESP = !bESP;
            if (bESP)
            {
                hESP = ESP(sdk);
                printf("TFI: Activated ESP\n");
            }
            else
            {
                printf("TFI: Deactivated ESP\n");
                PatchDirectX(hESP);
                hESP = nullptr;
            }
        }

        if (bAimbot)
            hook = (Hooking*)Aimbot(sdk, &currentFocusPlayer, &hooked);

        if (!bAimbot && hooked && hook != nullptr)
        {
            hook->Patch((BYTE*)sdk->CreateMoveAddress, sdk->ogCreateMove, 7);
            VirtualFree(hook->oCreateMove, hook->hookLength + 5, MEM_FREE);
            hook = nullptr;
            hooked = false;
        }

        if (bTriggerbot)
            Triggerbot(sdk);

        if (bBunnyhop)
            BunnyHop(sdk);
    }

    if (bESP && hESP != nullptr)
    {
        PatchDirectX(hESP);
        hESP = nullptr;
    }

    if (file != NULL) fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ConThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

