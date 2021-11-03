#pragma once

#define _USE_MATH_DEFINES
#define ABS(x) ((x < 0) ? (-x) : (x))

#include <Windows.h>
#include <iostream>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib");
#pragma comment(lib, "d3dx9.lib");

#include "pch.h"
#include "SDK.h"
#include "Offsets.h"
#include "PlayerEntity.h"
#include "Edges.h"
#include "Hooking.h"
#include "dx.h"
#include "Drawing.h"