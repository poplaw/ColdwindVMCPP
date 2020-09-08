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
		std::uint_fast8_t& operator[](int n);
		std::optional<std::uint_fast8_t> fetchByte(std::uint_fast32_t addr);
		bool storeByte(std::uint_fast32_t addr, std::uint_fast8_t value);
		std::optional<std::uint_fast32_t> fetchDword(std::uint_fast32_t addr);
		bool storeDword(std::uint_fast32_t addr, std::uint_fast32_t value);
		std::optional<std::vector<std::uint_fast8_t>> fetchMany(std::uint_fast32_t addr, std::uint_fast32_t size);
		bool storeMany(std::uint_fast32_t addr, const std::vector<std::uint_fast8_t>& array);
	};
}
