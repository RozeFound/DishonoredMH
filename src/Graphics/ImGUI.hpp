#pragma once

#include "pch.h"

class ImGUI_Renderer {

public:

	void Init(IDirect3DDevice9* pDevice);
	void LockInput(bool isInputLocked);
	void UpdateOverlayState(bool MenuOpen);

	void Begin();
	void End();

	void Destroy();

private:

	bool isInitialized = false;
	HWND window = nullptr;
};
