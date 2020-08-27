#include "Memory.h"

namespace VM
{


	std::optional<std::uint_fast8_t> Memory::fetchByte(std::int_fast32_t addr)
	{
		if ((addr < 0) || (addr >= mem.size()))
		{
			return std::nullopt;
		}

		return mem[addr];
	}

	bool Memory::storeByte(std::int_fast32_t addr, std::uint_fast8_t value)
	{
		if ((addr < 0) || (addr >= mem.size()))
			return false;

		mem[addr] = value;
		return true;
	}

	std::optional<std::uint_fast32_t> Memory::fetchDword(std::int_fast32_t addr)
	{
		if ((addr < 0) || (addr + 3 >= mem.size()))
		{
			return std::nullopt;
		}

		return (
			(mem[addr])				 |
			(mem[addr + 1] << 8)	 |
			(mem[addr + 2] << 16)	 |
			(mem[addr + 3] << 24)
		);
	}

	bool Memory::storeDword(std::int_fast32_t addr, std::uint_fast32_t value)
	{
		if ((addr < 0) || (addr + 3 >= mem.size()))
		{
			return false;
		}

		mem[addr] = value & 0xff;
		mem[addr + 1] = value & (value >> 8) & 0xff;
		mem[addr + 2] = value & (value >> 16) & 0xff;
		mem[addr + 3] = value & (value >> 24) & 0xff;

		return true;
	}

	std::optional<std::vector<std::uint_fast8_t>> Memory::fetchMany(std::int_fast32_t addr, std::int_fast32_t size)
	{
		if (addr + size - 1 >= mem.size())
		{
			return std::nullopt;
		}

		return std::vector<std::uint_fast8_t>(mem.begin() + addr, mem.begin() + addr + size);
	}

	bool Memory::storeMany(std::int_fast32_t addr, const std::vector<std::uint_fast8_t>& array)
	{
		if (addr + array.size() - 1 >= mem.size())
		{
			return false;
		}

		for (int i = 0; i < array.size(); ++i)
		{
			mem[addr + i] = array[i];
		}

		return true;
	}
}