#include "pch.h"
#include "Features.hpp"

void NopRestore::Enable() {

	std::memcpy(oldBytes.get(), (std::byte*)m_Address, m_Size);
	mem::in::Nop((std::byte*)m_Address, m_Size);
}

void NopRestore::Disable() {

	mem::in::Patch((std::byte*)m_Address, oldBytes.get(), m_Size);
}

void Hook::Enable() {

	m_GatewayAddr = (uintptr_t)mem::in::TrumpHook32((std::byte*)m_Address, (std::byte*)m_Destination, m_Size);
	AdditionalData = (void*)m_GatewayAddr;
}

void Hook::Disable() {

	mem::in::Patch((std::byte*)m_Address, (std::byte*)m_GatewayAddr, m_Size);
	VirtualFree((std::byte*)m_GatewayAddr, 0, MEM_RELEASE);
}
