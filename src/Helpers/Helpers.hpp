#pragma once

#include "pch.h"
#include "../Memory/Memory.hpp"

class Console {

public:

	Console() {
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	~Console() {
		fclose(f);
		FreeConsole();
	}

	void cls();

private:

	HANDLE hOut;
	FILE* f;
};

inline bool isKeyPressed(int vKey) {
	return GetAsyncKeyState(vKey) & 1;
}
std::string_view boolToSwitch(bool value);
std::wstring widen(std::string_view string);