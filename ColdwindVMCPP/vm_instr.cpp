#include "vm_instr.h"

#include <array>

namespace VM
{
	std::uint_fast16_t to_dw(std::array<std::uint_fast8_t, 2> args)
	{
		std::uint_fast16_t returnValue{ 0 };

		returnValue = (
			(returnValue | args[0]) |
			(returnValue | args[1]) << 8
		);

		return returnValue;
	}

	std::uint_fast32_t to_dd(std::array<std::uint_fast8_t, 4> args)
	{
		std::uint_fast32_t returnValue{ 0 };

		returnValue = (
			(returnValue | args[0]) |
			((returnValue | args[1]) << 8) | 
			((returnValue | args[2]) << 16) |
			((returnValue | args[3]) << 24)
		);

		return returnValue;
	}

	void VMOV(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = vm.reg(args[1]).value;
	}

	void VSET(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		std::array<std::uint_fast8_t, 4> buffer;
		for (int i{ 1 }; i < 5; ++i)
		{
			buffer[i - 1] = args[i];
		}

		vm.reg(args[0]).value = to_dd(buffer);
	}

	void VLD(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto dd = vm.mem.fetchDword(vm.reg(args[1]).value);

		if (dd)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}
		vm.reg(args[0]).value = dd.value();
	}

	void VST(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!vm.mem.storeDword(vm.reg(args[0]).value, vm.reg(args[1]).value))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
		}
	}

	void VLDB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto db = vm.mem.fetchByte(vm.reg(args[1]).value);
		if (!db)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}
		vm.reg(args[0]).value = db.value();
	}

	void VSTB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!vm.mem.storeByte(vm.reg(args[0]).value, vm.reg(args[1]).value & 0xff))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
		}
	}

	void VADD(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value + vm.reg(args[1]).value) & 0xffffffff);
	}

	void VXOR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value ^= vm.reg(args[1]).value;
	}

	void VCMP(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto res = vm.reg(args[0]).value - vm.reg(args[1]).value;
		vm.fr &= 0xfffffffc;

		if (res == 0)
		{
			vm.fr |= vm.FLAG_ZF;
		}

		if (res < 0)
		{
			vm.fr |= vm.FLAG_CF;
		}
	}

	void VJZ(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (vm.fr & vm.FLAG_ZF)
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VOUTB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto port = args[1];

		for (const auto& element : args)
		{
			if (port == element)
			{
				vm.io[port]->handleInbound(port, vm.reg(args[0]).value & 0xff);
				return;
			}
		}

		return;
	}

	void VJMP(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		std::array<std::uint_fast8_t, 2> buffer;
		for (int i{ 0 }; i < 2; ++i)
		{
			buffer[i] = args[i];
		}

		vm.pc.value = (vm.pc.value + to_dw(buffer)) & 0xffff;
	}

	void VOFF(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.terminated = true;
	}

	std::map <std::int_fast16_t, InstructionSignature> vm_opcodes = {
		{ 0x00, {VMOV, 1 + 1}},
		{ 0x01, {VSET, 1 + 4}},
		{ 0x02, {VLD, 1 + 1}},
		{ 0x03, {VST, 1 + 1}},
		{ 0x04, {VLDB, 1 + 1}},
		{ 0x05, {VSTB, 1 + 1}},
		{ 0x10, {VADD, 1 + 1}},
		{ 0x17, {VXOR, 1 + 1}},
		{ 0x20, {VCMP, 1 + 1}},
		{ 0x21, {VJZ, 2}},
		{ 0x40, {VJMP, 2}},
		{ 0xF2, {VOUTB, 1 + 1}},
		{ 0xFF, {VOFF, 0}}
	};
}