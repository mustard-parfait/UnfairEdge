#pragma once

// Game classes redeclared
class RecvTable;
class RecvProp;
class ClientClass;

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vec4;

// Re-definition of RecvProp from "dt_recv.h#L87"
class RecvProp
{
public:
	const char* m_pVarName;
	void* m_RecvType;
	int	m_Flags;
	int	m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int	m_Offset;
	int	m_ElementStride;
	int	m_nElements;
	const char* m_pParentArrayPropName;
};

// Re-definition of RecvTable from "dt_recv.h#L172"
class RecvTable
{
public:
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	const char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

// Re-definition of ClientClass from "client_class.h#L49"
class ClientClass
{
public:
	void* m_pCreatefn;
	void* m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};

// Re-definition of IBaseClientDLL from "cdll_int.h#L584"
class IBaseClientDLL
{
public:
	virtual void func_pad0() = 0;
	virtual void func_pad1() = 0;
	virtual void func_pad2() = 0;
	virtual void func_pad3() = 0;
	virtual void func_pad4() = 0;
	virtual void func_pad5() = 0;
	virtual void func_pad6() = 0;
	virtual void func_pad7() = 0;

	// GetAllClasses() - returns a ClientClass*
	virtual ClientClass* GetAllClasses() = 0;
};

// Re-definition of IClientEntityList from "icliententitylist.h#L28"
class IClientEntityList
{
public:
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableHandle(int hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

	virtual void* GetClientEntity(int entnum) = 0;
	virtual void* GetClientEntityFromHandle(int hEnt) = 0;

	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void SetMaxEntities(int maxents) = 0;
	virtual int GetMaxEntities() = 0;
};

class CUserCmd {
public:
	int pad;
	int command_number;
	int tick_count;
	Vec3 viewangles;
	Vec3 aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	char impulse;
	int weaponselect;
	int weaponsubtype;
	int randomseed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
};

// SDK for required entities
class SDK
{
public:
	IBaseClientDLL* BaseClientDLL;
	ClientClass* clientClassAddr;
	IClientEntityList* ClientEntityList;
	void* ClientModeInterface;
	void* ClientMode;
	void* CreateMoveAddress;
	BYTE ogCreateMove[7]{ 0 };

	SDK();
	ClientClass* GetClientClass();
	uintptr_t GetPlayerEntity(int num);
	uintptr_t GetLocalPlayerEntity();
	bool GameToScreen(Vec3 Game, Vec2& Screen);
};

// Definition from TF2 source code
typedef void* (*tCreateInterface)(const char* moduleName, int* returnCode);

// Create and get interface for mentioned module and interface
void* GetInterface(const wchar_t* moduleName, const char* interfaceName);

// Definition for DirectX Scene
typedef HRESULT(APIENTRY* xEndScene)(LPDIRECT3DDEVICE9 xDevice);