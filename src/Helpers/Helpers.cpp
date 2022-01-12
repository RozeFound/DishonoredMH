#include "pch.h"
#include "Helpers.hpp"

void Console::cls() {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	std::cout.flush();

	GetConsoleScreenBufferInfo(hOut, &csbi);
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;

	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

std::string_view boolToSwitch(bool value) {
	if (value) return "> ON < ";
	else return "> OFF <";
}

std::wstring widen(std::string_view string) {

	std::wstring result;

	wchar_t wch;

	for (char ch : string) {
		size_t len = std::mbtowc(&wch, &ch, 4);
		result.append(1, wch);
	}

	return result;
}

//void MoveInCameraDirection(vec3 angles, vec3& src, float distance) {
//
//	auto DTR = [](float degree) {return degree * (float)(std::numbers::pi / 180); };
//
//	src.x += std::cosf(DTR(angles.x - 90)) * distance;
//	src.y += std::sinf(DTR(angles.x - 90)) * distance;
//	src.z += std::sinf(DTR(angles.y)) * distance;
//}