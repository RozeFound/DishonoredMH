#pragma once

#include "pch.h"
#include "../Process/Process.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9d.lib")

typedef HRESULT(APIENTRY* _EndScene)(IDirect3DDevice9* pDevice);
bool GetD3D9Device(void** pTable, size_t size);