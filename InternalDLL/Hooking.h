#pragma once

// Define CreateMove function
typedef bool(__thiscall* fCreateMove)(float, CUserCmd*);

class Hooking
{
public:
	fCreateMove oCreateMove = nullptr;
	int hookLength = 7;

	Hooking();
	Hooking(LocalPlayer* lpPlayer);
	void SetLocalPlayer(LocalPlayer* lpPlayer);
	
	void Patch(BYTE* dst, BYTE* src, DWORD size);
	bool InlineHook(char* src, char* dst, DWORD size);
	char* TrampolineHook(char* src, char* dst, DWORD size);
};

// Declare fake CreateMove function
bool __fastcall fakeCreateMove(float sampletime, CUserCmd* usercmd);

// Declare fake DirectX Scene
void APIENTRY fakeDirectXScene(LPDIRECT3DDEVICE9 ogDevice);