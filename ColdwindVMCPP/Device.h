#pragma once
#include <cstdint>

class Device
{
public:
	virtual void handleInbound(std::int_fast16_t port, std::uint_fast8_t byte) = 0;
	virtual char handleOutbound(std::int_fast16_t port) = 0;
};

