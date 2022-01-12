#include "pch.h"
#include "ImGUI.hpp"

static LONG_PTR oWndProc = 0;
static bool lockInput = false;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (lockInput) {
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return true;
	}

	return CallWindowProc((WNDPROC)oWndProc, hWnd, msg, wParam, lParam);
}

void ImGUI_Renderer::Init(IDirect3DDevice9* pDevice) {

	if (!isInitialized) {

		ImGui::CreateContext();

		D3DDEVICE_CREATION_PARAMETERS CP = {};
		pDevice->GetCreationParameters(&CP);
		window = CP.hFocusWindow;

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.IniFilename = NULL;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.Fonts->AddFontDefault();

		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = .5f;

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);
		isInitialized = true;

		oWndProc = SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
	}
}

void ImGUI_Renderer::LockInput(bool isInputLocked) {
	lockInput = isInputLocked;
}

void ImGUI_Renderer::UpdateOverlayState(bool MenuOpen) {

	LONG_PTR style = GetWindowLongPtr(window, GWL_EXSTYLE);

	if (MenuOpen) {
		style &= ~WS_EX_LAYERED;
		SetWindowLongPtr(window, GWL_EXSTYLE, style);
		SetForegroundWindow(window);
	}
	else {
		style |= WS_EX_LAYERED;
		SetWindowLongPtr(window, GWL_EXSTYLE, style);
	}

}

void ImGUI_Renderer::Begin() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGUI_Renderer::End() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImGUI_Renderer::Destroy() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)oWndProc);
}