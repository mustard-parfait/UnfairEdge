#pragma once

// Definition for original DirectX Scene
typedef HRESULT(APIENTRY* fnDirectXScene)(LPDIRECT3DDEVICE9 lpDevice);

// To access game window and window data
static HWND ProcessWindow;
extern int WindowHeight, WindowWidth;
extern LPDIRECT3DDEVICE9 lpDevice;

// Get game process window
HWND GetProcessWindow();

// Get DirectX Device - true if found device
bool GetDirectXDevice(void** lpTable, size_t size);