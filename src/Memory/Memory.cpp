#include "pch.h"
#include "Memory.hpp"

namespace mem {

	int hardException(unsigned int code, DWORD EXCEPTION_TYPE) {
		if (code == EXCEPTION_TYPE) {
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else return EXCEPTION_CONTINUE_SEARCH;
	}

	namespace in {

		void Patch(std::byte* dst, std::byte* src, size_t size) {

			DWORD oldProtect;
			VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
			std::memcpy(dst, src, size);
			VirtualProtect(dst, size, oldProtect, &oldProtect);
		}

		void Nop(std::byte* dst, size_t size) {
			auto NopArray = std::make_unique<std::byte[]>(size);
			std::memset(NopArray.get(), 0x90, size);
			Patch(dst, NopArray.get(), size);
		}

		uintptr_t FindDMAAddy(uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets) {

			uintptr_t tempAddress = basePointer;

			for (const auto& offset : offsets) {

				tempAddress = *(uintptr_t*)tempAddress;
				tempAddress += offset;
			}

			return tempAddress;
		}

		uintptr_t FindDMAAddy_s(uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets) {

			uintptr_t retAddress;

			__try {
				retAddress = FindDMAAddy(basePointer, offsets);
			}
			__except (hardException(GetExceptionCode(), EXCEPTION_ACCESS_VIOLATION)) {
				retAddress = 0;
			}

			return retAddress;
		}

		bool Detour32(std::byte* src, std::byte* dst, const uintptr_t len) {

			if (len < 5) return false;

			DWORD oldProtect;
			VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

			std::memset(src, 0x90, len);

			uintptr_t relativeAddres = dst - src - 5;

			*src = (std::byte)0xE9;
			*(uintptr_t*)(src + 1) = relativeAddres;

			VirtualProtect(src, len, oldProtect, &oldProtect);

			return true;
		}

		std::byte* TrumpHook32(std::byte* src, std::byte* dst, const uintptr_t len) {

			if (len < 5) return 0;

			std::byte* gateway = (std::byte*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			memcpy_s(gateway, len, src, len);

			uintptr_t gatewayRelativeAddr = src - gateway - 5;

			*(gateway + len) = (std::byte)0xE9;
			*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

			Detour32(src, dst, len);

			return gateway;
		}
	}

	namespace ext {

		size_t WriteMemory(HANDLE processHandle, uintptr_t address, auto value, size_t size) {

			size_t bytes_writen = 0;
			size_t _size = size ? size : sizeof(value);
			WriteProcessMemory(processHandle, (void*)address, &value, _size, (SIZE_T*)&bytes_writen);
			return bytes_writen;
		}

		void Patch(HANDLE processHandle, std::byte* dst, std::byte* src, size_t size) {

			DWORD oldProtect;
			VirtualProtectEx(processHandle, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
			WriteMemory(processHandle, (uintptr_t)dst, src, size);
			VirtualProtectEx(processHandle, dst, size, oldProtect, &oldProtect);
		}

		void Nop(HANDLE processHandle, std::byte* dst, size_t size) {
			auto NopArray = std::make_unique<std::byte[]>(size);
			std::memset(NopArray.get(), 0x90, size);
			Patch(processHandle, dst, NopArray.get(), size);
		}

		uintptr_t FindDMAAddy(HANDLE processHandle, uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets) {

			uintptr_t tempAddress = basePointer;

			for (const auto& offset : offsets) {

				tempAddress = ReadMemory(processHandle, tempAddress);
				tempAddress += offset;
			}

			return tempAddress;
		}
	}
}