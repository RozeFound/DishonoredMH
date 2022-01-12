#pragma once

#include "pch.h"
#include "Helpers.hpp"
#include "Constants.hpp"
#include "Geometry.hpp"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {std::byte MAKE_PAD(offset); type name;}

struct Description {

	union {
		DEFINE_MEMBER_N(wchar_t*, Name, 0x38);
		DEFINE_MEMBER_N(wchar_t*, nName, 0x44);
		DEFINE_MEMBER_N(wchar_t*, strDescription, 0x50);
		DEFINE_MEMBER_N(wchar_t*, strID, 0x64);
	};
};

struct Item {

	Description* desc;
	int Value;
	int Unknown;
};

struct Entity {

	struct EnityData {

		union {
			DEFINE_MEMBER_N(Entity*, localPlayer, 0x24);
			DEFINE_MEMBER_N(uintptr_t, inventory, 0xC8);
		};
	};

	union {
		DEFINE_MEMBER_N(rotmat, RotationMatrix, 0x50);
		DEFINE_MEMBER_N(vec3_t, Position, 0xC4);
		DEFINE_MEMBER_N(int, Health, 0x344);
		DEFINE_MEMBER_N(int, maxHealth, 0x348);
		DEFINE_MEMBER_N(int, Mana, 0xA60);
		DEFINE_MEMBER_N(int, maxMana, 0xA64);
		DEFINE_MEMBER_N(EnityData*, entData, 0x59C);
	};
};

Item* GetItemByStrID(Entity* player, std::string_view strId) {

	if (player->entData->inventory)
		for (auto& item : *(std::array<Item, 100>*)player->entData->inventory)
			if (item.desc->strID && !wcscmp(item.desc->strID, widen(strId).c_str()))
				return &item;

	return nullptr;
}