#include "pch.h"
#include "Include.h"

int WindowHeight, WindowWidth;

// Loop through all windows to find process window
BOOL CALLBACK LoopWindows(HWND handle, LPARAM lp)
{
	DWORD ProcID;

	// Keep on looping until process ID matches current process ID
	GetWindowThreadProcessId(handle, &ProcID);
	if (GetCurrentProcessId() != ProcID)
		return TRUE;

	ProcessWindow = handle;
	return FALSE;
}

// Get window handle and window data
HWND GetProcessWindow()
{
	ProcessWindow = NULL;
	
	EnumWindows(LoopWindows, NULL);
	if (ProcessWindow == NULL) return NULL;

	RECT WindRect;
	GetWindowRect(ProcessWindow, &WindRect);

	// Set window height and width
	WindowHeight = WindRect.bottom - WindRect.top;
	WindowWidth = WindRect.right - WindRect.left;
	WindowHeight = 720;
	WindowWidth = 1280;

	return ProcessWindow;
}

// Get DirectX Device - true if found device
bool GetDirectXDevice(void** lpTable, size_t size)
{
	// Return false if lpTable is NULL
	if (!lpTable)
		return false;

	// Create a Direct3D Inteface
	IDirect3D9* Direct3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (!Direct3DInterface) return false;

	// Initiate a fake Direct3D Device
	IDirect3DDevice9* fakeD3DDevice = nullptr;

	// Initiate D3D paramaters for fake Direct3D Device
	D3DPRESENT_PARAMETERS D3DParameter = {};
	D3DParameter.Windowed = false;
	D3DParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DParameter.hDeviceWindow = GetProcessWindow();

	// Create fake device
	HRESULT D3DFake = Direct3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParameter.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParameter, &fakeD3DDevice);
	if (D3DFake != S_OK)
	{
		// Change windowed status
		D3DParameter.Windowed = !D3DParameter.Windowed;
		HRESULT D3DFake = Direct3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParameter.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParameter, &fakeD3DDevice);

		// Return false if not ok
		if (D3DFake != S_OK)
		{
			Direct3DInterface->Release();
			return false;
		}
	}

	// If created copy device in lpTable and release fake device & interface
	memcpy(lpTable, *(void***)(fakeD3DDevice), size);
	fakeD3DDevice->Release();
	Direct3DInterface->Release();
	return true;
}