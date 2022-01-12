#pragma once

#include "pch.h"

namespace mem {

	namespace in {

		void Nop(std::byte* dst, size_t size);
		void Patch(std::byte* dst, std::byte* src, size_t size);

		uintptr_t FindDMAAddy(uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets);
		uintptr_t FindDMAAddy_s(uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets);

		bool Detour32(std::byte* src, std::byte* dst, const uintptr_t len);
		std::byte* TrumpHook32(std::byte* src, std::byte* dst, const uintptr_t len);
	}

	namespace ext {

		class ReadMemory {

		public:

			ReadMemory(HANDLE processHandle, uintptr_t address, size_t size = 0) :
				m_ProcessHandle(processHandle), m_Address(address), m_Size(size) {}

			template<typename T> operator T() {

				T data = {};

				size_t size = m_Size ? m_Size : sizeof(data);
				ReadProcessMemory(m_ProcessHandle, (void*)m_Address, &data, size, nullptr);

				return data;
			}

			template<typename T> size_t operator=(T value) {
				return WriteMemory(m_ProcessHandle, m_Address, value, m_Size);
			}

		private:

			HANDLE m_ProcessHandle;
			uintptr_t m_Address;
			size_t m_Size;
		};
		size_t WriteMemory(HANDLE processHandle, uintptr_t address, auto value, size_t size = 0);

		void Patch(HANDLE processHandle, std::byte* dst, std::byte* src, size_t size);
		void Nop(HANDLE processHandle, std::byte* dst, size_t size);

		uintptr_t FindDMAAddy(HANDLE processHandle, uintptr_t basePointer, std::initializer_list<std::ptrdiff_t> offsets);
	}
}
