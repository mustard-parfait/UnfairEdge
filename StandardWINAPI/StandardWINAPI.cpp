#include "Include.h"

int main()
{
	printf("TF2 External\n");
	SDK* sdk = new SDK(L"hl2.exe");
	Offsets* offsets = new Offsets(sdk);
	PlayerOffsets* pOffsets = new PlayerOffsets(sdk);
	
	bool bAimbot = false, bTriggerbot = false, bBunnyhop = false;
	uintptr_t currentFocusPlayer = 0;

	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bAimbot = !bAimbot;
			currentFocusPlayer = 0;
			if (bAimbot)
				printf("TFI: Activated Aim Bot\n");
			else
				printf("TFI: Deactivated Aim Bot\n");
		}

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bTriggerbot = !bTriggerbot;
			if (bTriggerbot)
				printf("TFI: Activated Trigger Bot\n");
			else
				printf("TFI: Deactivated Trigger Bot\n");
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
			GlowObject(sdk, offsets, pOffsets);

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
			RocketJump(sdk, offsets, pOffsets, 1);

		if (GetAsyncKeyState(VK_NUMPAD8) & 1)
			RocketJump(sdk, offsets, pOffsets, 2);

		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
			RocketJump(sdk, offsets, pOffsets, 3);

		if (GetAsyncKeyState(VK_NUMPAD7) & 1)
		{
			bBunnyhop = !bBunnyhop;
			if (bBunnyhop)
				printf("TFI: Activated Bunny Hop\n");
			else
				printf("TFI: Deactivated Bunny Hop\n");
		}

		if (bAimbot)
			Aimbot(sdk, &currentFocusPlayer, offsets, pOffsets);

		if (bTriggerbot)
			Triggerbot(sdk, offsets, pOffsets);

		if (bBunnyhop)
			BunnyHop(sdk, offsets, pOffsets);
	}
}