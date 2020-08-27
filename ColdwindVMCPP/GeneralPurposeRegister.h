#pragma once

#include <cstdint>

namespace VM
{
	class GeneralPurposeRegister
	{
	public:
		std::int_fast32_t value;
		GeneralPurposeRegister();
	};
}

