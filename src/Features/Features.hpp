#pragma once

#include "pch.h"
#include "../Memory/Memory.hpp"


class Feature {

public:

	Feature() = default;
	virtual ~Feature() {
		this->Switch(false);
	}

	virtual void Switch(bool state) {

		if (state && !_isEnabled) {
			Enable();
			_isEnabled = true;
		}
		else if (!state && _isEnabled) {
			Disable();
			_isEnabled = false;
		}
	}

	void* AdditionalData = nullptr;

protected:

	virtual void Enable() = 0;
	virtual void Disable() = 0;

private:

	bool _isEnabled = false;
};

class NopRestore : public Feature {

public:

	NopRestore() = default;
	NopRestore(uintptr_t address, size_t size) :
		m_Address(address), m_Size(size), oldBytes(new std::byte[size]) {}

	void Enable();
	void Disable();

private:

	std::unique_ptr<std::byte[]> oldBytes;
	uintptr_t m_Address;
	size_t m_Size;
};

class Hook : public Feature {

public:

	Hook() = default;
	Hook(uintptr_t sourceAddress, uintptr_t destinationAddress, size_t size)
		: m_Address(sourceAddress), m_Destination(destinationAddress), m_Size(size) {}

	void Enable();
	void Disable();

private:

	uintptr_t m_Address;
	uintptr_t m_Destination;
	uintptr_t m_GatewayAddr = 0;
	size_t m_Size;
};


