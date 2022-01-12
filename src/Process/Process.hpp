#pragma once

#include "pch.h"
#include "Helpers.hpp"

namespace proc {

	DWORD GetProcessID(std::string_view processName);
	uintptr_t GetModuleBaseAddress(DWORD processId, std::string_view moduleName);
	HWND GetProcessWindow();

}