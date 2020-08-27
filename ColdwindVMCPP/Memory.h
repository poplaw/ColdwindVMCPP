#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <optional>

namespace VM
{
	class Memory
	{
		std::array<std::uint_fast8_t, 64 * 1024> mem;

	public:
		std::optional<std::uint_fast8_t> fetchByte(std::int_fast32_t addr);
		bool storeByte(std::int_fast32_t addr, std::uint_fast8_t value);
		std::optional<std::uint_fast32_t> fetchDword(std::int_fast32_t addr);
		bool storeDword(std::int_fast32_t addr, std::uint_fast32_t value);
		std::optional<std::vector<std::uint_fast8_t>> fetchMany(std::int_fast32_t addr, std::int_fast32_t size);
		bool storeMany(std::int_fast32_t addr, const std::vector<std::uint_fast8_t>& array);
	};
}
