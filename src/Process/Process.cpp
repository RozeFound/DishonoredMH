#include "pch.h"
#include "Process.hpp"

namespace proc {

	DWORD GetProcessID(std::string_view processName) {

		std::wstring wProcName = widen(processName);

		DWORD processId = 0;
		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snapshotHandle != INVALID_HANDLE_VALUE) {

			PROCESSENTRY32 procEntry;
			procEntry.dwSize = sizeof(procEntry);

			if (Process32First(snapshotHandle, &procEntry)) {

				do {
					if (!_wcsicmp(procEntry.szExeFile, wProcName.c_str())) {
						processId = procEntry.th32ProcessID;
						break;
					}
				} while (Process32Next(snapshotHandle, &procEntry));
			}
		}
		CloseHandle(snapshotHandle);
		return processId;
	}

	uintptr_t GetModuleBaseAddress(DWORD processId, std::string_view moduleName) {

		// I hate wide characters
		std::wstring wModName = widen(moduleName);

		uintptr_t moduleBaseAddress = 0;
		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

		if (snapshotHandle != INVALID_HANDLE_VALUE) {

			MODULEENTRY32 moduleEntry;
			moduleEntry.dwSize = sizeof(moduleEntry);

			if (Module32First(snapshotHandle, &moduleEntry)) {
				do {
					if (!_wcsicmp(moduleEntry.szModule, wModName.c_str())) {
						moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
						break;
					}
				} while (Module32Next(snapshotHandle, &moduleEntry));
			}
		}
		CloseHandle(snapshotHandle);
		return moduleBaseAddress;
	}

	HWND GetProcessWindow() {

		static HWND resultWindow;

		auto callback = [](HWND windowHandle, LPARAM lParam) -> BOOL {
			DWORD windowProcessID;
			GetWindowThreadProcessId(windowHandle, &windowProcessID);

			if (GetCurrentProcessId() != windowProcessID)
				return TRUE;

			resultWindow = windowHandle;
			return FALSE;
		};

		EnumWindows(callback, NULL);
		return resultWindow;
	}
}