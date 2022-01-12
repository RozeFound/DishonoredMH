#include "pch.h"

#include "GameStructures.hpp"

#include "Process/Process.hpp"
#include "Memory/Memory.hpp"
#include "Geometry.hpp"
#include "Helpers.hpp"

#include "Features/Features.hpp"
#include "Graphics/d3d9_hook.hpp"
#include "Graphics/ImGUI.hpp"

struct Vars {

	uintptr_t moduleBase = NULL;
	std::unordered_map<std::string_view, std::unique_ptr<Feature>> features;

	vec3_t PositionToTeleport = {};

	bool bInvincibility = false;
	bool bInfinityMana = false;
	bool bInfinityMoney = false;
	bool MenuOpen = false;

	Entity* localPlayer = nullptr;

	int fakeMoney = 1337;
	int* moneyPtr = &fakeMoney;
} vars;

ImGUI_Renderer renderer;
_EndScene EndSceneGateway = nullptr;
void* d3d9Device[119];

void HandleKeys() {

	if (isKeyPressed(VK_NUMPAD1)) {
		vars.bInvincibility = !vars.bInvincibility;
	}
	if (isKeyPressed(VK_NUMPAD2)) {
		vars.bInfinityMana = !vars.bInfinityMana;	
	}
	if (isKeyPressed(VK_NUMPAD3)) {
		vars.bInfinityMoney = !vars.bInfinityMoney;	
	}
	if (isKeyPressed(VK_NUMPAD4)) {
		vars.PositionToTeleport = vars.localPlayer->Position;
	}
	if (isKeyPressed(VK_NUMPAD5)) {
		vars.PositionToTeleport = {};
	}
	if (isKeyPressed(VK_NUMPAD6)) {
		vars.localPlayer->Position = vars.PositionToTeleport;
	}
	if (isKeyPressed(VK_HOME)) {
		vars.MenuOpen = !vars.MenuOpen;
		renderer.LockInput(vars.MenuOpen);
	}
}

void DrawMenu() {

	ImGui::Begin("DishonoredMH", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (vars.localPlayer) {
		if (ImGui::CollapsingHeader("Info"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::InputInt("Health", &vars.localPlayer->Health, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::InputInt("Mana", &vars.localPlayer->Mana, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::InputInt("Money", vars.moneyPtr, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
		}
		if (ImGui::CollapsingHeader("Functions"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Checkbox("Invincibility (NUM1)", &vars.bInvincibility);
			ImGui::Checkbox("Infinity Mana (NUM2)", &vars.bInfinityMana);
			ImGui::Checkbox("Infinity Money (NUM3)", &vars.bInfinityMoney);
		}
		if (ImGui::CollapsingHeader("Teleport"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Text("Saved Coords: [%s]", vars.PositionToTeleport.to_string().c_str());
			if (ImGui::Button("Save Position (NUM4)"))
				vars.PositionToTeleport = vars.localPlayer->Position;
			if (ImGui::Button("Reset Position (NUM5)"))
				vars.PositionToTeleport = {};
			if (ImGui::Button("Teleport To Saved Position (NUM6)"))
				vars.localPlayer->Position = vars.PositionToTeleport;
		}
	}
	else {
		ImGui::Text("Enter the game, please.");
	}
	ImGui::End();
}

HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pD3DDevice) {

	renderer.Init(pD3DDevice);

	if (*(int*)(vars.moduleBase + 0x1073c7c) == 2) { // 1 - in Menu/Loading | 2 - in game

		vars.localPlayer = *(Entity**)(vars.moduleBase + 0x1052de8);

// 		if (vars.localPlayer->entData->inventory) {
// 			for (auto& item : *(std::array<Item, 100>*)vars.localPlayer->entData->inventory)
// 				if (item.desc->strID && !wcscmp(item.desc->strID, L"Money"))
// 					vars.moneyPtr = &item.Value;
// 		}
// 		else vars.moneyPtr = &vars.fakeMoney;

		if (auto* item = GetItemByStrID(vars.localPlayer, "Money")) 
			vars.moneyPtr = &(*item).Value;
		else vars.moneyPtr = &vars.fakeMoney;
	}

	vars.features["invincibility"]->Switch(vars.bInvincibility);
	vars.features["infinityMana"]->Switch(vars.bInfinityMana);
	vars.features["infinityMoney"]->Switch(vars.bInfinityMoney);

	HandleKeys();

	renderer.Begin(); // Draw pull starts here

	if (vars.MenuOpen) DrawMenu();

	{ // DEBUG
		ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		if (vars.localPlayer) {

			auto viewMatrix = *(mat4x4_t*)(vars.moduleBase + 0x1066670);
			auto projectionMatrix = *(mat4x4_t*)(vars.moduleBase + 0x1066670 + 0x60);

			vec3_t position = {};
			auto mvpMatrix = viewMatrix * projectionMatrix;

			D3DVIEWPORT9 viewport;
			pD3DDevice->GetViewport(&viewport);
			vec2_t screenResolution = { (float)viewport.Height, (float)viewport.Width };

			if (auto screen = WorldToScreen(position, mvpMatrix, screenResolution)) {
				ImGui::Text("ScreenPos: %f, %f", screen->to_string().c_str());
				auto draw = ImGui::GetBackgroundDrawList();
				auto color = ImColor(255.f, 255.f, 255.f, 1.f);
				draw->AddText(*screen, color, "Hi There");
			}
		}

		ImGui::End();
	}

	renderer.End(); // Render all the shit

	return EndSceneGateway(pD3DDevice);
}

void CleanUp() {
	renderer.Destroy();
	for (auto& [key, feature] : vars.features)
		feature->Switch(false);
	EndSceneGateway = (_EndScene)d3d9Device[42];
}

int InitHooks(HMODULE hModule) {

	Sleep(1000);

	wchar_t label[64];
	HWND window = proc::GetProcessWindow();
	GetWindowText(window, label, 64);

	vars.moduleBase = (uintptr_t)GetModuleHandle(NULL);

	vars.features["invincibility"] = std::make_unique<NopRestore>(vars.moduleBase + 0x183fa9, 6);
	vars.features["infinityMana"] = std::make_unique<NopRestore>(vars.moduleBase + 0x6b1815, 6);
	vars.features["infinityMoney"] = std::make_unique<NopRestore>(vars.moduleBase + 0x80f53c, 3);

	while (true) {
		if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {

			vars.features["endSceneHook"] = std::make_unique<Hook>((uintptr_t)d3d9Device[42], (uintptr_t)hkEndScene, 7);
			vars.features["endSceneHook"]->Switch(true);
			EndSceneGateway = (_EndScene)vars.features["endSceneHook"]->AdditionalData;

			SetWindowText(window, std::format(L"{} - Injected", label).c_str());
			break;
		}
	}

	while(!isKeyPressed(VK_END))
		Sleep(1);

	CleanUp();
	SetWindowText(window, label);
	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {

	DisableThreadLibraryCalls(hModule);

    if (dwReason == DLL_PROCESS_ATTACH) {
		HANDLE ThreadHandle = nullptr;
		ThreadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitHooks, hModule, 0, nullptr);
		if(ThreadHandle) CloseHandle(ThreadHandle);  
    }

    return TRUE;
}
