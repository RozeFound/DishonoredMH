#pragma once

#include "pch.h"

typedef void(__thiscall* _DecreaseMoney)(void* _this, int a1, int moneySpent);
typedef int(__thiscall* _OpenInventory)(void* _this, int a1, int* a2);

typedef BOOL(WINAPI* _SetCursorPos)(int X, int Y);
typedef BOOL(WINAPI* _GetCursorPos)(LPPOINT lpPoint);

std::string_view menu =

"########################################\n"
"#         Dishonored MultiHack         #\n"
"#                                      #\n"
"#      NUM1 - Invincibility  {}   #\n"
"#      NUM2 - Infinity Mana  {}   #\n"
"#      NUM3 - Infinity Money {}   #\n"
"#      NUM4 - Double Money             #\n"
"#      NUM5 - Set Teleport Coords      #\n"
"#      NUM6 - Goto Teleport Coodrs     #\n"
"#                                      #\n"
"#            END - To Exit             #\n"
"########################################\n"

"\nSaved coords: [{}, {}, {}]\n"
"Your Health = {}, Your Mana = {}, Your Money = {}\n";
